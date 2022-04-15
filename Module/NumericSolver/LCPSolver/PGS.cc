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

	double merit_current = dbl_max;
	double merit_previous = dbl_max;

	int step = 0;
	bool diverge = false;
	while (step++ < _max_step) {
		merit_previous = merit_current;
		merit_current = 0;
		for (int i = 0; i < size; i++) {
			double ri = A.row(i).dot(x) + b(i);
			x(i) = std::max(0.0, x(i) - _lambda * ri / A(i, i));
			if (x(i) > merit_current) {
				merit_current = x(i);
			}
 		}
		if (merit_current > merit_previous) {
			diverge = true;
			break;
		}
		if ((merit_previous - merit_current) / merit_current < _max_error) {
			break;
		}
	}

//	std::cerr << "x: \n" << x.transpose() << std::endl;
//	std::cerr << "y: \n" << (A * x + b).transpose() << std::endl;

	if (step < _max_step + 1 && !diverge) {
		spdlog::info("PGS method, converge in {} steps", step);
	} else {
		spdlog::warn("PGS method, fail to converge");
	}

	return x;
}
