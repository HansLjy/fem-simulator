//
// Created by hansljy on 2022/3/21.
//

#include "NewtonIterator.h"
#include "Util/EigenAll.h"
#include <spdlog/spdlog.h>

using Eigen::ConjugateGradient;

NewtonIteratorParameter::NewtonIteratorParameter(double max_error, int max_step,
												 double armijo,
												 double curvature)
												 : OptimizerParameter(max_error, max_step),
												 _armijo(armijo), _curvature(curvature){}

DEFINE_ACCESSIBLE_MEMBER(NewtonIteratorParameter, double, Armijo, _armijo)
DEFINE_ACCESSIBLE_MEMBER(NewtonIteratorParameter, double, Curvature, _curvature)

DEFINE_CLONE(OptimizerParameter, NewtonIteratorParameter)
DEFINE_CLONE(Optimizer, NewtonIterator)

void NewtonIterator::Initialize(const OptimizerParameter &para) {
	Optimizer::Initialize(para);
	_armijo = para.GetArmijo();
}

VectorXd NewtonIterator::Optimize(const VectorXd &x0) const {
	VectorXd x = x0;
	VectorXd I(x.size());
	I.setConstant(1);

	int step = 0;
	ConjugateGradient<SparseMatrixXd> solver;
	while (step++ < _max_step) {
//		spdlog::info("Newton Iterator, round {}", step);
		SparseMatrixXd hessian = _target->Hessian(x);

		double alpha = 0.002;
		solver.compute(hessian);
		while (solver.info() != Eigen::Success) {
			alpha *= 2;
			hessian.diagonal() += alpha * I;
			solver.compute(hessian);
		}
		spdlog::info("Change into SPD matrix, alpha = {}", alpha);

		VectorXd gradient = _target->Gradient(x);
		VectorXd p = -solver.solve(gradient);

		// Line search (backtracking)
		double t = 1;
		double fx = _target->Value(x);
		double dfp = gradient.dot(p);

		while (t > 0.1 && _target->Value(x + t * p) >= fx + alpha * dfp * t) {
//			spdlog::info("Still not converge, t = {}", t);
			t *= 0.9;
		}

		x += t * p;

		if (gradient.norm() < _max_error) {
			break;
		}
		spdlog::info("Error: {}, continue iterating", gradient.norm());
	}
	if (step <= _max_step) {
		spdlog::info("Newton Method converges in {} steps", step);
	} else {
		spdlog::warn("Newton Method fails to converge");
	}
	return x;
}