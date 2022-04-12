//
// Created by hansljy on 2022/4/9.
//

#include "PGS.h"
#include <iostream>
#include <spdlog/spdlog.h>

DEFINE_ACCESSIBLE_MEMBER(PGSParameter, double, Lambda, _lambda)

VectorXd PGS::Solve(const MatrixXd &A, const VectorXd &b,
					const VectorXd &x0) const {

	std::cerr << "A: \n" << A << std::endl << "b: \n" << b.transpose() << std::endl;
	const int size = b.size();

	int step = 0;

	VectorXd x = x0;
	if (x.size() == 0) {
		x.resizeLike(b);
	}
	while (step++ < _max_step) {
		for (int i = 0; i < size; i++) {
			double ri = A.row(i).dot(x) + b(i);
			x(i) = std::max(0.0, x(i) - _lambda * ri / A(i, i));
 		}
		VectorXd y = A * x + b;
		if (y.minCoeff() >= 0 && x.dot(y) < _max_error)
		if ((A * x + b).minCoeff() >= 0) {
			break;
		}
		std::cerr << step << ": " << x.transpose() << std::endl;
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
