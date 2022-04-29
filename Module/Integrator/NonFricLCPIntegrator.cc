//
// Created by hansljy on 2022/4/26.
//

#include "NonFricLCPIntegrator.h"
#include <spdlog/spdlog.h>
#include <Eigen/CholmodSupport>

void NonFricLCPIntegrator::Step(System &system, const ContactGenerator &contact,
								const BodyEnergy &body_energy, double h) {
	const int num_tangent = contact.GetNumTangent();

	SparseMatrixXd JnT, JtT;
	VectorXd Mu;

	contact.GetContact(system, JnT, JtT, Mu);

	const int num_contact = JnT.rows();
	spdlog::info("Number of contact points: {}", num_contact);

	int size = system.GetCoordSize();	// size for u, x, M and all such matrices
	SparseMatrixXd W(size, size);

	auto t = clock();
	VectorXd M, u, f;
	system.GetSysMass(M);
	system.GetSysV(u);
	system.GetSysF(f, body_energy);
	system.GetSysPFPX(W, body_energy);
	spdlog::info("Time spent on calculation phy-info: {}s", (clock() - t) / CLOCKS_PER_SEC);

	W *= h * h;
	W.diagonal() += M;
	VectorXd c = M.asDiagonal() * u + h * f;

	t = clock();
	double alpha = 0.01;

	Eigen::CholmodSupernodalLLT<SparseMatrixXd> LLT_solver;
	LLT_solver.compute(W);
	while (LLT_solver.info() != Eigen::Success) {
		spdlog::info("Making W SPD");
		W.diagonal() += alpha * M;
		alpha *= 2;
		LLT_solver.compute(W);
	}

	auto sub_t = clock();
	MatrixXd WiJn = LLT_solver.solve(JnT.transpose().toDense());
	MatrixXd WiJt = LLT_solver.solve(JtT.transpose().toDense());
	spdlog::info("Time spent solving J: {}", clock() - sub_t);
	sub_t = clock();
	VectorXd Wic = LLT_solver.solve(c);
	spdlog::info("Time spent solving c: {}", clock() - sub_t);

	spdlog::info("Time spent on solving linear equations: {}s", (clock() - t) / CLOCKS_PER_SEC);

	MatrixXd E(num_contact * num_tangent, num_contact);
	for (int i = 0; i < num_contact; i++) {
		E.block(i * num_tangent, i, num_tangent, 1).setOnes();
	}

	t = clock();
	const int block_size = 1 + num_tangent;
	const int sizeA = num_contact * block_size;
	MatrixXd A(sizeA, sizeA);
	VectorXd b(sizeA);

	A.setZero();
	b.setZero();

	/* Non-blocking version */
	A.block(0, 0, num_contact, num_contact) = JnT * WiJn;
	A.block(0, num_contact, num_contact, num_contact * num_tangent) = JnT * WiJt;
	A.block(num_contact, 0, num_contact * num_tangent, num_contact) = JtT * WiJn;
	A.block(num_contact, num_contact, num_contact * num_tangent, num_contact * num_tangent) = JtT * WiJt;
	b.block(0, 0, num_contact, 1) = JnT * Wic;
	b.block(num_contact, 0, num_contact * num_tangent, 1) = JtT * Wic;

	spdlog::info("Time spent on assembling: {}s", (clock() - t) / CLOCKS_PER_SEC);

//	std::cerr << "A: \n" << A << std::endl;
//	std::cerr << "b: \n" << b.transpose() << std::endl;

	t = clock();

	static VectorXd sol;
	if (sol.size() == sizeA) {
		sol = _solver->Solve(A, b, sol, num_contact * (num_tangent + 1));
	} else {
		sol = _solver->Solve(A, b, VectorXd(sizeA), num_contact * (num_tangent + 1));
	}

	VectorXd lambda_n(num_contact), lambda_t(num_contact * num_tangent); // beta(num_contact);

	/* Non-blocking version */
	lambda_n = sol.block(0, 0, num_contact, 1);
	lambda_t = sol.block(num_contact, 0, num_contact * num_tangent, 1);

	spdlog::info("Time spent on de-assembling: {}s", (clock() - t) / CLOCKS_PER_SEC);


	VectorXd u_plus = Wic + WiJn * lambda_n + WiJt * lambda_t;

	system.Update(u_plus, h);
}