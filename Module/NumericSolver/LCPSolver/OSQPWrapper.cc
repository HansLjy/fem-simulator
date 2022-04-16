//
// Created by hansljy on 2022/4/15.
//

#include "OSQPWrapper.h"
#include <OsqpEigen/OsqpEigen.h>
#include <spdlog/spdlog.h>

DEFINE_CLONE(LCPSolverParameter, OSQPWrapperParameter)

VectorXd
OSQPWrapper::Solve(const MatrixXd &A, const VectorXd &b, const VectorXd &x0,
				   int block_size) const {
	const int size = b.size();
	if (size == 0) {
		return VectorXd(0);
	}
	SparseMatrixXd A_sparse = A.sparseView();
	VectorXd b_tmp = b;

	SparseMatrixXd constraints(size, size);
	std::vector<Triplet> COO;
	for (int i = 0; i < size; i++) {
		COO.push_back(Triplet(i, i, 1));
	}
	constraints.setFromTriplets(COO.begin(), COO.end());

	VectorXd lower_bound(size), upper_bound(size);
	for (int i = 0; i < size; i++) {
		lower_bound(i) = 0;
		upper_bound(i) = OsqpEigen::INFTY;
	}

	OsqpEigen::Solver solver;
	solver.settings()->setWarmStart(true);
	solver.data()->setNumberOfVariables(size);
	spdlog::info("Successfully set solver n = {}", size);
	solver.data()->setNumberOfConstraints(size);
	spdlog::info("Successfully set solver m = {}", size);
	solver.data()->setHessianMatrix(A_sparse);
	spdlog::info("Successfully set solver hessian");
	solver.data()->setGradient(b_tmp);
	spdlog::info("Successfully set solver gradient");
	solver.data()->setLinearConstraintsMatrix(constraints);
	solver.data()->setLowerBound(lower_bound);
	solver.data()->setUpperBound(upper_bound);
	spdlog::info("Successfully set solver constraints");

	solver.initSolver();
	auto t = clock();
	solver.solveProblem();
	spdlog::info("Time spent on solving LCP: {}s", (clock() - t) / CLOCKS_PER_SEC);
	return solver.getSolution();
}