//
// Created by hansljy on 2022/4/9.
//

#include "PGS.h"

DEFINE_ACCESSIBLE_MEMBER(PGSParameter, double, Lambda, _lambda)

VectorXd PGS::Solve(const SparseMatrixXd &A, const VectorXd &b,
					const VectorXd &x0) const {
	const int size = b.size();

	int step = 0;
	VectorXd x = x0;
	while (step++ < _max_step) {
		for (int i = 0; i < size; i++) {
			double ri = A.row(i).dot(x) + b(i);
			x(i) = std::max(0.0, x(i) - ri / A.coeff(i, i));
 		}
		if ((A * x + b).minCoeff() >= 0) {
			break;
		}
	}

	return x;
}
