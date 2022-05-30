//
// Created by hansljy on 2022/4/26.
//

#include "StaggerLCPIntegrator.h"
#include <OsqpEigen/OsqpEigen.h>
#include <Eigen/CholmodSupport>
#include <spdlog/spdlog.h>
#include "Util/Factory.h"
#include "Util/Timing.h"

void StaggerLCPIntegrator::Initialize(const IntegratorParameter &para) {
	LCPIntegrator::Initialize(para);
	_max_step = para.GetLCPSolverParameter()->GetMaxStep();
	_max_error = para.GetLCPSolverParameter()->GetMaxError();
}

void StaggerLCPIntegrator::Step(System &system,
								const ContactGenerator &contact_generator,
								const FrictionModel &friction_model, double h) {
	SparseMatrixXd JnT, JtT;
	VectorXd Mu;

	vector<ContactPoint> contacts;
	contact_generator.GetContact(system, contacts);
//	for(auto& contact : contacts) {
//		std::cerr << "Collision between " << contact._obj1 << " and " << contact._obj2 << std::endl;
//		std::cerr << "Point: " << contact._point.transpose() << "\nNormal: " << contact._normal.transpose() << std::endl;
//	}
	friction_model.GetJ(system, contacts, 0, JnT, JtT, Mu);
//	std::cerr << "Jn:\n" << JnT.transpose() << std::endl;
//	std::cerr << "Jt:\n" << JtT.transpose() << std::endl;
	const int num_tangent = friction_model.GetNumTangent();

	const int num_contact = JnT.rows();
	spdlog::info("Number of contact points: {}", num_contact);

	int dof = system.GetSysDOF();	// dof for u, x, M and all such matrices
	SparseMatrixXd W(dof, dof);

	VectorXd u, f;
	SparseMatrixXd mass = system.GetSysMass();
	system.GetSysV(u);
	system.GetSysF(f);
	system.GetSysEnergyHessian(W);

//	std::cerr << f.transpose() << std::endl;
//	std::cerr << W.toDense() << std::endl;
//	std::cerr << mass.toDense() << std::endl;

	W *= h * h;
	W += mass;
	VectorXd c = mass * u + h * f;

	Eigen::CholmodSupernodalLLT<SparseMatrixXd> LLT_solver;
	LLT_solver.compute(W);
	double alpha = 0.01;
	while (LLT_solver.info() != Eigen::Success) {
		spdlog::info("Making W SPD");
		W += alpha * mass;
		alpha *= 2;
		LLT_solver.compute(W);
	}

	START_TIMING(t)
	MatrixXd WiJn = LLT_solver.solve(JnT.transpose().toDense());
	MatrixXd WiJt = LLT_solver.solve(JtT.transpose().toDense());
	VectorXd Wic = LLT_solver.solve(c);
	STOP_TIMING_TICK(t, "solving linear equations")


	MatrixXd E(num_contact * num_tangent, num_contact);
	E.setZero();
	for (int i = 0; i < num_contact; i++) {
		E.block(i * num_tangent, i, num_tangent, 1).setOnes();
	}

	MatrixXd Ann = JnT * WiJn, Ant = JnT * WiJt, Atn = JtT * WiJn, Att = JtT * WiJt;

	SparseMatrixXd contact_hessian_sparse = Ann.sparseView();
	SparseMatrixXd friction_hessian_sparse = Att.sparseView();

	VectorXd bn = JnT * Wic;
	VectorXd bt = JtT * Wic;

//	std::cerr << "A: \n" << A << std::endl;
//	std::cerr << "b: \n" << b.transpose() << std::endl;

	static VectorXd xn(0), xt(0);
	if (xn.size() != num_contact) {
		xn.resize(num_contact);
		xn.setZero();
		xt.resize(num_contact * num_tangent);
		xt.setZero();
	}

	if (num_contact != 0) {
		OsqpEigen::Solver contact_solver, friction_solver;
		contact_solver.settings()->setWarmStart(true);
		contact_solver.data()->setNumberOfVariables(num_contact);
		contact_solver.data()->setNumberOfConstraints(num_contact);
		contact_solver.data()->setHessianMatrix(contact_hessian_sparse);
		SparseMatrixXd contact_constraint(num_contact, num_contact);
		vector<Triplet> contact_constraint_COO;
		for (int i = 0; i < num_contact; i++) {
			contact_constraint_COO.push_back(Triplet(i, i, 1));
		}
		contact_constraint.setFromTriplets(contact_constraint_COO.begin(),
										   contact_constraint_COO.end());
		contact_solver.data()->setLinearConstraintsMatrix(contact_constraint);
		VectorXd contact_lower_bound(num_contact), contact_upper_bound(
				num_contact);
		for (int i = 0; i < num_contact; i++) {
			contact_lower_bound(i) = 0;
			contact_upper_bound(i) = OsqpEigen::INFTY;
		}
		contact_solver.data()->setLowerBound(contact_lower_bound);
		contact_solver.data()->setUpperBound(contact_upper_bound);
		VectorXd contact_gradient = Ant * xt + bn;
		contact_solver.data()->setGradient(contact_gradient);

		const int friction_constraint_size = num_contact * (num_tangent + 1);
		const int friction_variable_size = num_contact * num_tangent;
		friction_solver.settings()->setWarmStart(true);
		friction_solver.data()->setNumberOfVariables(friction_variable_size);
		friction_solver.data()->setNumberOfConstraints(
				friction_constraint_size);
		friction_solver.data()->setHessianMatrix(friction_hessian_sparse);
		SparseMatrixXd friction_constraint(friction_constraint_size,
										   friction_variable_size);
		vector<Triplet> friction_constraint_COO;
		for (int i = 0; i < friction_variable_size; i++) {
			friction_constraint_COO.push_back(Triplet(i, i, 1));
		}
		for (int i = 0; i < num_contact; i++) {
			const int base_i = i + num_contact * num_tangent;
			const int base_j = num_tangent * i;
			for (int j = 0; j < num_tangent; j++) {
				friction_constraint_COO.push_back(
						Triplet(base_i, base_j + j, -1));
			}
		}
		friction_constraint.setFromTriplets(friction_constraint_COO.begin(),
											friction_constraint_COO.end());
		friction_solver.data()->setLinearConstraintsMatrix(friction_constraint);
		VectorXd friction_lower_bound(
				friction_constraint_size), friction_upper_bound(
				friction_constraint_size);
		for (int i = 0; i < friction_variable_size; i++) {
			friction_lower_bound(i) = 0;
			friction_upper_bound(i) = OsqpEigen::INFTY;
		}
		for (int i = friction_variable_size;
			 i < friction_constraint_size; i++) {
			friction_upper_bound(i) = OsqpEigen::INFTY;
		}
		friction_lower_bound.block(friction_variable_size, 0, num_contact,
								   1) = -(Mu.asDiagonal() * xn);
		friction_solver.data()->setLowerBound(friction_lower_bound);
		friction_solver.data()->setUpperBound(friction_upper_bound);
		VectorXd friction_gradient = Atn * xn + bt;
		friction_solver.data()->setGradient(friction_gradient);

		contact_solver.initSolver();
		friction_solver.initSolver();

		int step = 0;
		VectorXd pre_xn = xn, pre_xt = xt;
		START_TIMING(t_iter)
		while (step++ < _max_step) {
			contact_solver.updateGradient(bn + Ant * xt);
			contact_solver.solveProblem();
			xn = contact_solver.getSolution();
			friction_solver.updateGradient(bt + Atn * xn);
			friction_lower_bound.block(friction_variable_size, 0, num_contact,
									   1) = -(Mu.asDiagonal() * xn);
			friction_solver.updateLowerBound(friction_lower_bound);
			friction_solver.solveProblem();
			xt = friction_solver.getSolution();
			if ((xn - pre_xn).norm() < _max_error &&
				(xt - pre_xt).norm() < _max_error) {
				break;
			}
			pre_xn = xn;
			pre_xt = xt;
		}
		STOP_TIMING_SEC(t_iter, "Staggering iteration")
		if (step <= _max_step) {
			spdlog::info("Staggering Method, converges in {} steps", step);
		} else {
			spdlog::warn("Staggering Method, does not converge");
		}
	}

//	if (num_contact > 0) {
//		std::cerr << "Normal Force: " << xn.transpose() << std::endl;
//		std::cerr << "Friction Force: " << xt.transpose() << std::endl;
//	}

	VectorXd u_plus = Wic + WiJn * xn + WiJt * xt;

	system.UpdateDynamic(u_plus, h);
}