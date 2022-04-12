//
// Created by hansljy on 2022/4/9.
//

#include "LCPIntegrator.h"
#include <Eigen/IterativeLinearSolvers>

void LCPIntegrator::Step(System &system, const ContactGenerator &contact,
						 const BodyEnergy &body_energy, double h,
						 const Solver &solver) {
	auto& ext_force = system.GetExternalForces();
	const int num_ext = ext_force.size();

	auto& soft_bodies = system.GetSoftBodies();
	const int num_soft = soft_bodies.size();

	const int num_tangent = contact.GetNumTangent();

	SparseMatrixXd JnT, JtT;
	VectorXd Mu;

	contact.GetContact(system, JnT, JtT, Mu);

	const int num_contact = JnT.rows();

	// TODO: add dissipation force

	int size = JnT.cols();	// size for u, x, M and all such matrices
	VectorXd M(size), u(size), f(size);
	SparseMatrixXd W(size, size);

	int current_size = 0;
	for (int i = 0; i < num_soft; i++) {
		int single_size = soft_bodies[i]._sparse_mass.size();
		M.block(current_size, 0, single_size, 1) = soft_bodies[i]._sparse_mass;
		u.block(current_size, 0, single_size, 1) = soft_bodies[i]._v;
		f.block(current_size, 0, single_size, 1).setZero();
		for (int j = 0; j < num_ext; j++) {
			f.block(current_size, 0, single_size, 1) -= ext_force[j]->Gradient(soft_bodies[i]);
		}
		f.block(current_size, 0, single_size, 1) -= body_energy.EGradient(soft_bodies[i]);
		W.block(current_size, current_size, single_size, single_size) = h * h * body_energy.EHessian(soft_bodies[i]);
		current_size += single_size;
	}

	VectorXd c = M.asDiagonal() * u + h * f;
	W.diagonal() += M;

	Eigen::BiCGSTAB<SparseMatrixXd> linear_solver;
	linear_solver.compute(W);

	SparseMatrixXd WiJn = linear_solver.solve(JnT.transpose());
	SparseMatrixXd WiJt = linear_solver.solve(JtT.transpose());
	VectorXd Wic = linear_solver.solve(c);

	SparseMatrixXd E(num_contact * num_tangent, num_contact);
	for (int i = 0; i < num_contact; i++) {
		E.block(i * num_tangent, i, num_tangent, 1) = VectorXd::Ones();
	}

	const int sizeA = num_contact * (2 + num_tangent);
	SparseMatrixXd A(sizeA, sizeA);
	VectorXd b(sizeA);

	A.block(0, 0, num_contact, num_contact) = JnT * WiJn;
	A.block(0, num_contact, num_contact, num_tangent * num_contact) = JnT * WiJt;
	A.block(num_contact, 0, num_tangent * num_contact, num_contact) = JtT * WiJn;
	A.block(num_contact, num_contact, num_tangent * num_contact, num_tangent * num_contact) = JtT * WiJt;

	A.block(num_contact * (num_tangent + 1), 0, num_contact, num_contact) = Mu;
	A.block(num_contact * (num_tangent + 1), num_contact, num_contact, num_contact * num_tangent) = - E.transpose();
	A.block(num_contact, num_contact * (num_tangent + 1), num_tangent * num_contact, num_contact) = E;

	b.block(0, 0, num_contact, 1) = JnT * Wic;
	b.block(num_contact, 0, num_contact * num_tangent, 1) = JtT * Wic;

	VectorXd sol = solver.LCP->Solve(A, b);

	VectorXd u_plus = Wic + WiJn * sol.block(0, 0, num_contact, 1) + WiJt * sol.block(num_contact, 0, num_contact * num_tangent, 1);

	current_size = 0;
	for (int i = 0; i < num_soft; i++) {
		int size_single = soft_bodies[i]._v.size();
		soft_bodies[i]._mesh.GetPoints() += h * u_plus.block(current_size, 0, size_single, 1);
		soft_bodies[i]._v = u_plus.block(current_size, 0, size_single, 1);
		current_size += size_single;
	}
}