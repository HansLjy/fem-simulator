//
// Created by hansljy on 2022/4/9.
//

#include "LCPIntegrator.h"
#include "Util/Factory.h"
#include <Eigen/IterativeLinearSolvers>
#include <spdlog/spdlog.h>
#include <iostream>

DEFINE_CLONE(IntegratorParameter, LCPIntegratorParameter)
DEFINE_ACCESSIBLE_MEMBER(LCPIntegratorParameter, LCPSolverType, LCPSolverType, _lcp_type)
DEFINE_ACCESSIBLE_POINTER_MEMBER(LCPIntegratorParameter, LCPSolverParameter, LCPSolverParameter, _lcp_para)

void LCPIntegrator::Initialize(const IntegratorParameter &para) {
	_solver = LCPSolverFactory::GetInstance()->GetLCPSolver(para.GetLCPSolverType());
	_solver->Initialize(*para.GetLCPSolverParameter());
}

void LCPIntegrator::Step(System &system, const ContactGenerator &contact,
						 const BodyEnergy &body_energy, double h) {
	const int num_tangent = contact.GetNumTangent();

	SparseMatrixXd JnT, JtT;
	VectorXd Mu;

	contact.GetContact(system, JnT, JtT, Mu);

	const int num_contact = JnT.rows();

	int size = system.GetCoordSize();	// size for u, x, M and all such matrices
	SparseMatrixXd W(size, size);

	VectorXd M, u, f;
	system.GetSysMass(M);
	system.GetSysV(u);
	system.GetSysF(f, body_energy);
	system.GetSysPFPX(W, body_energy);

	W *= h * h;
	W.diagonal() += M;
	VectorXd c = M.asDiagonal() * u + h * f;

	double alpha = 0.01;
	Eigen::ConjugateGradient<SparseMatrixXd> PCG_solver;
	PCG_solver.compute(W);
	while (PCG_solver.info() != Eigen::Success) {
		spdlog::info("Making W SPD");
		W.diagonal() += alpha * M;
		PCG_solver.compute(W);
	}

	MatrixXd WiJn = PCG_solver.solve(JnT.transpose().toDense());
	MatrixXd WiJt = PCG_solver.solve(JtT.transpose().toDense());
	VectorXd Wic = PCG_solver.solve(c);

	MatrixXd E(num_contact * num_tangent, num_contact);
	for (int i = 0; i < num_contact; i++) {
		E.block(i * num_tangent, i, num_tangent, 1).setOnes();
	}

	const int block_size = 1 + num_tangent;
	const int sizeA = num_contact * block_size;
	MatrixXd A(sizeA, sizeA);
	VectorXd b(sizeA);

	A.setZero();
	b.setZero();

	/* test A is SPD or not */

	MatrixXd Wi = W.toDense().inverse();

	A.block(0, 0, num_contact, num_contact) = JnT * Wi * JnT.transpose();
	A.block(0, num_contact, num_contact, num_contact * num_tangent) = JnT * Wi * JtT.transpose();
	A.block(num_contact, 0, num_contact * num_tangent, num_contact) = JtT * Wi * JnT.transpose();
	A.block(num_contact, num_contact, num_contact * num_tangent, num_contact * num_tangent) = JtT * Wi * JtT.transpose();

	Eigen::LLT<MatrixXd> llt;
	llt.compute(W);
	if (llt.info() == Eigen::NumericalIssue) {
		spdlog::error("W is not SPD!");
		exit(-1);
	}
	llt.compute(A);
	if (llt.info() == Eigen::NumericalIssue) {
		spdlog::error("A is not SPD!");
		exit(-1);
	}


	for (int i = 0; i < num_contact; i++) {
		const int base_i = i * block_size;
		for (int j = 0; j < num_contact; j++) {
			const int base_j = j * block_size;
			A(base_i, base_j) = JnT.row(i) * WiJn.col(j);
			A.block(base_i, base_j + 1, 1, num_tangent) = JnT.row(i) * WiJt.block(0, j * num_tangent, size, num_tangent);
			A.block(base_i + 1, base_j, num_tangent, 1) = JtT.block(i * num_tangent, 0, num_tangent, size) * WiJn.col(j);
			A.block(base_i + 1, base_j + 1, num_tangent, num_tangent) = JtT.block(i * num_tangent, 0, num_tangent, size) * WiJt.block(0, j * num_tangent, size, num_tangent);
		}
//		A(base_i + 1 + num_tangent, base_i) = Mu(i);
//		A.block(base_i + 1 + num_tangent, base_i + 1, 1, num_tangent).setConstant(-1);
//		A.block(base_i + 1, base_i + 1 + num_tangent, num_tangent, 1).setConstant(1);
	}

	for (int i = 0; i < num_contact; i++) {
		const int base_i = i * block_size;
		b(base_i) = JnT.row(i) * Wic;
		b.block(base_i + 1, 0, num_tangent, 1) = JtT.block(i * num_tangent, 0, num_tangent, size) * Wic;
	}

//	std::cerr << "A: \n" << A << std::endl;
//	std::cerr << "b: \n" << b.transpose() << std::endl;

	VectorXd sol = _solver->Solve(A, b, VectorXd(sizeA), block_size);
	VectorXd lambda_n(num_contact), lambda_t(num_contact * num_tangent); // beta(num_contact);
	for (int i = 0; i < num_contact; i++) {
		lambda_n(i) = sol(i * block_size);
		lambda_t(Eigen::seqN(i * num_tangent, num_tangent)) = sol(Eigen::seqN(i * block_size + 1, num_tangent));
//		beta(i) = sol(i * block_size + 1 + num_tangent);
	}

	VectorXd u_plus = Wic + WiJn * lambda_n + WiJt * lambda_t;

	system.Update(u_plus, h);
}