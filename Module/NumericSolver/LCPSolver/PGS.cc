//
// Created by hansljy on 2022/4/9.
//

#include "PGS.h"
#include <iostream>
#include <spdlog/spdlog.h>
#include <limits>

DEFINE_CLONE(LCPSolverParameter, PGSParameter)
DEFINE_ACCESSIBLE_MEMBER(PGSParameter, double, Lambda, _lambda)

VectorXd PGS::Solve(const MatrixXd &A, const VectorXd &b, const VectorXd &x0,
					int block_size) const {
//	std::cerr << "A: \n" << A << std::endl << "b: \n" << b.transpose() << std::endl;
	const int size = b.size();

	VectorXd x = x0;

	const double dbl_max = std::numeric_limits<double>::max();

	int step = 0;
	while (step++ < _max_step) {
		double inf_norm = 0;
		for (int i = 0; i < size; i++) {
			double ri = A.row(i).dot(x) + b(i);
			if (A(i, i) < _max_error && A(i, i) > -_max_error) {
				spdlog::error("Zero diagonal number at ({}, {})!", i, i);
				exit(0);
			}
			x(i) = std::max(0.0, x(i) - _lambda * ri / A(i, i));
			if (x(i) > inf_norm) {
				inf_norm = x(i);
			}
 		}
		VectorXd y = A * x + b;
		double min_y = 0, max_violate = 0;
		for (int i = 0; i < size; i++) {
			min_y = std::min(min_y, y(i));
			max_violate = std::max(max_violate, std::min(x(i), y(i)));
		}
		if (min_y > -inf_norm * _max_error && inf_norm != 0 && max_violate / inf_norm < _max_error) {
			break;
		}
	}

//	std::cerr << "x: \n" << x.transpose() << std::endl;
//	std::cerr << "y: \n" << (A * x + b).transpose() << std::endl;

	if (step < _max_step + 1) {
		spdlog::info("PGS method, converge in {} steps", step);
	} else {
		spdlog::warn("PGS method, fail to converge");
	}

	return x;
}
