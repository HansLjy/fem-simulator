//
// Created by hansljy on 2022/4/9.
//

#include "PGS.h"
#include <iostream>
#include <spdlog/spdlog.h>

DEFINE_CLONE(LCPSolverParameter, PGSParameter)
DEFINE_ACCESSIBLE_MEMBER(PGSParameter, double, Lambda, _lambda)

VectorXd PGS::Solve(const MatrixXd &A, const VectorXd &b, const VectorXd &x0,
					int block_size) const {
	std::cerr << "A: \n" << A << std::endl << "b: \n" << b.transpose() << std::endl;
	const int size = b.size();

	VectorXd x = x0;

	int step = 0;
	while (step++ < _max_step) {
		for (int i = 0; i < size; i++) {
			double ri = A.row(i).dot(x) + b(i);
			x(i) = std::max(0.0, x(i) - _lambda * ri / A(i, i));
 		}
		VectorXd y = A * x + b;
		bool y_ok = true;
		for (int i = 0; i < size; i++) {
			if (y(i) < 0) {
				y_ok = false;
			}
		}
		if (y_ok && x.dot(y) < _max_error) {
			break;
		}
	}

	std::cerr << "x: \n" << x.transpose() << std::endl;
	std::cerr << "y: \n" << (A * x + b).transpose() << std::endl;

	if (step < _max_step + 1) {
		spdlog::info("PGS method, converge in {} steps", step);
	} else {
		spdlog::warn("PGS method, fail to converge");
	}

	return x;
}
