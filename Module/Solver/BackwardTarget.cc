//
// Created by hansljy on 2022/2/22.
//

#include "BackwardTarget.h"
#include "BodyEnergy/BodyEnergy.h"
#include "Eigen/Sparse"

double BackwardTarget::Value(const VectorXd &x) const {
	VectorXd v = (x - _x) / _dt;
	auto M = _reference.GetMass().asDiagonal();
	return 0.5 * (v - _v).transpose() * M * (v - _v)
		+ _body_energy->EEnergy(_reference, x)
		+ _body_energy->DEnergy(_reference, _x, v);
}


VectorXd BackwardTarget::Gradient(const VectorXd &x) const {
	VectorXd v = (x - _x) / _dt;
	auto M = _reference.GetMass().asDiagonal();
	return 1.0 / _dt * M * (v - _v)
		+ _body_energy->EGradient(_reference, x)
		+ _body_energy->DGradient(_reference, _x, v);
}

MatrixXd BackwardTarget::Hessian(const VectorXd &x) const {
	VectorXd v = (x - _x) / _dt;
	auto M = _reference.GetMass().asDiagonal().toDenseMatrix();
	return 1 / (_dt * _dt) * M
		+ _body_energy->EHessian(_reference, x)
		+ _body_energy->DHessian(_reference, _x, v);
}

DEFINE_CLONE(Function, BackwardTarget)