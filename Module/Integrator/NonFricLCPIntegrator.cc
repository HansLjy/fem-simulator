//
// Created by hansljy on 2022/4/26.
//

#include "NonFricLCPIntegrator.h"
#include <spdlog/spdlog.h>
#include <Eigen/CholmodSupport>
#include "Util/Timing.h"

void NonFricLCPIntegrator::Step(System &system,
								const ContactGenerator &contact_generator,
								const FrictionModel &friction_model, double h) {
//	const int num_tangent = contact_generator.GetNumTangent();
//
//	SparseMatrixXd JnT, JtT;
//	VectorXd Mu;
//
//	contact_generator.GetContact(system, );
//
//	const int num_contact = JnT.rows();
//	spdlog::info("Number of contact points: {}", num_contact);
//
//	int size = system.GetCoordSize();	// size for u, x, M and all such matrices
//	SparseMatrixXd W(size, size);
//
//	VectorXd M, u, f;
//	system.GetSysMass(M);
//	system.GetSysV(u);
//	system.GetSysF(f);
//	system.GetSysEnergyHessian(W);
//
//	W *= h * h;
//	W.diagonal() += M;
//	VectorXd c = M.asDiagonal() * u + h * f;
//
//	double alpha = 0.01;
//
//	Eigen::CholmodSupernodalLLT<SparseMatrixXd> LLT_solver;
//	LLT_solver.compute(W);
//	while (LLT_solver.info() != Eigen::Success) {
//		spdlog::info("Making W SPD");
//		W.diagonal() += alpha * M;
//		alpha *= 2;
//		LLT_solver.compute(W);
//	}
//
//	START_TIMING(t_solving)
//	MatrixXd WiJn = LLT_solver.solve(JnT.transpose().toDense());
//	MatrixXd WiJt = LLT_solver.solve(JtT.transpose().toDense());
//	VectorXd Wic = LLT_solver.solve(c);
//	STOP_TIMING_TICK(t_solving, "solving linear equations")
//
//	MatrixXd E(num_contact * num_tangent, num_contact);
//	for (int i = 0; i < num_contact; i++) {
//		E.block(i * num_tangent, i, num_tangent, 1).setOnes();
//	}
//
//	const int block_size = 1 + num_tangent;
//	const int sizeA = num_contact * block_size;
//	MatrixXd A(sizeA, sizeA);
//	VectorXd b(sizeA);
//
//	A.setZero();
//	b.setZero();
//
//	/* Non-blocking version */
//	A.block(0, 0, num_contact, num_contact) = JnT * WiJn;
//	A.block(0, num_contact, num_contact, num_contact * num_tangent) = JnT * WiJt;
//	A.block(num_contact, 0, num_contact * num_tangent, num_contact) = JtT * WiJn;
//	A.block(num_contact, num_contact, num_contact * num_tangent, num_contact * num_tangent) = JtT * WiJt;
//	b.block(0, 0, num_contact, 1) = JnT * Wic;
//	b.block(num_contact, 0, num_contact * num_tangent, 1) = JtT * Wic;
//
//	static VectorXd sol;
//	if (sol.size() == sizeA) {
//		sol = _solver->Solve(A, b, sol, num_contact * (num_tangent + 1));
//	} else {
//		sol = _solver->Solve(A, b, VectorXd(sizeA), num_contact * (num_tangent + 1));
//	}
//
//	VectorXd lambda_n(num_contact), lambda_t(num_contact * num_tangent);
//
//	/* Non-blocking version */
//	lambda_n = sol.block(0, 0, num_contact, 1);
//	lambda_t = sol.block(num_contact, 0, num_contact * num_tangent, 1);
//
//	VectorXd u_plus = Wic + WiJn * lambda_n + WiJt * lambda_t;
//
//	system.UpdateDynamic(u_plus, h);
}