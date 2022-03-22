//
// Created by hansljy on 2022/3/21.
//

#include "NumericDifferential.h"

NumericDifferential::NumericDifferential(double delta) : _delta(delta) {}

void NumericDifferential::SetFuntion(const VecFunction& func) {
	_func = func;
}

VectorXd NumericDifferential::NumGradient(VectorXd x) {
	int n = x.size();
	VectorXd gradient(n);
	for (int i = 0; i < n; i++) {
		const double tmp = x(i);
		x(i) = tmp + _delta;
		double f_plus = _func(x);
		x(i) = tmp - _delta;
		double f_minus = _func(x);
		x(i) = tmp;
		gradient(i) = (f_plus - f_minus) / (2 * _delta);
	}
	return gradient;
}

MatrixXd NumericDifferential::NumHessian(VectorXd x) {
	int n = x.size();
	MatrixXd hessian(n, n);
	const double delta[] = {-_delta, +_delta};
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			double f[2][2];
			double tmpi = x(i), tmpj = x(j);
			for (int di = 0; di < 2; di++) {
				for (int dj = 0; dj < 2; dj++) {
					x(i) = tmpi + delta[di];
					x(j) = tmpj + delta[dj];
					f[di][dj] = _func(x);
				}
			}
			x(i) = tmpi;
			x(j) = tmpj;
			hessian(i, j) = (f[1][1] - f[0][1] - f[1][0] + f[0][0]) / (4 * _delta * _delta);
		}
		double tmp = x(i);
		x(i) = tmp + _delta;
		double f_plus = _func(x);
		x(i) = tmp - _delta;
		double f_minus = _func(x);
		x(i) = tmp;
		double f = _func(x);
		hessian(i, i) = (f_minus + f_plus - 2 * f) / (_delta * _delta);
	}
	return hessian;
}