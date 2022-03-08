//
// Created by hansljy on 2022/2/24.
//

#include "InteriorPoint.h"
#include <spdlog/spdlog.h>

InteriorPointParameter::InteriorPointParameter(double max_error, int max_step,
											   double mu) : OptimizerParameter(max_error, max_step), _mu(mu){}

DEFINE_CLONE(OptimizerParameter, InteriorPointParameter)

VectorXd InteriorPoint::Optimize(const VectorXd &x0) const {
	int m = _constraints.size();	// number of constraints
	int n = x0.size();				// number of x
	VectorXd lambda(m), one(m);
	for (int i = 0; i < m; i++) {
		lambda(i) = _mu / _constraints[i]->Value(x0);
		one(i) = 1;
	}
	VectorXd x = x0;
	int step = 0;
	while (step++ < _max_step) {

		// TODO equation needed (need to type math here)

		MatrixXd W = _target->Hessian(x);
		for (int i = 0; i < m; i++) {
			W -= lambda(i) * _constraints[i]->Hessian(x);
		}
		MatrixXd A(n, m);
		for (int i = 0; i < m; i++) {
			A.col(i) = _constraints[i]->Gradient(x);
		}
		VectorXd G(m);
		for (int i = 0; i < m; i++) {
			G(i) = _constraints[i]->Value(x);
		}
		MatrixXd LHS(n + m, n + m);
		LHS.block(0, 0, n, n) = W;
		LHS.block(0, n, n, m) = -A;
		LHS.block(n, 0, m, n) = lambda.asDiagonal() * A.transpose();
		LHS.block(n, n, m, m) = G.asDiagonal().toDenseMatrix();
		VectorXd RHS(n + m);
		RHS.block(0, 0, n, 1) = -_target->Gradient(x) + A * lambda;
		RHS.block(n, 0, m, 1) = _mu * one - G.asDiagonal() * lambda;

		VectorXd sol = LHS.colPivHouseholderQr().solve(RHS);

		double alpha = 1;
		for (int i = 0; i < m; i++) {
			if (sol(n + i) < 0) {
				alpha = std::min(alpha, lambda(i) / (- sol(n + i) * 2));
			}
		}

		x += alpha * sol.block(0, 0, n, 1);
		lambda += alpha * sol.block(n, 0, m, 1);

		if (sol.norm() < _max_error) {
			break;
		}
	}

	if (step <= _max_step) {
		spdlog::info("Interior Point Optimizer, converge in {} step", step - 1);
	} else {
		spdlog::warn("Interior Point Optimizer, fail to converge");
	}
	return x;
}

DEFINE_CLONE(Optimizer, InteriorPoint)