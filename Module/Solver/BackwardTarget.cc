//
// Created by hansljy on 2022/2/22.
//

#include "BackwardTarget.h"
#include "BodyEnergy/BodyEnergy.h"
#include "Eigen/Sparse"

double BackwardTarget::Value(const VectorXd &x) const {
	VectorXd v = (x - _x) / _dt;
	auto M = _mass.asDiagonal();
	return 0.5 * (v - _v).transpose() * M * (v - _v)
		+ _body_energy->EEnergy(_reference, _volumn, _inv, x)
		+ _body_energy->DEnergy(_reference, _volumn, _mass, _inv, _x, v);
}


VectorXd BackwardTarget::Gradient(const VectorXd &x) const {
	VectorXd v = (x - _x) / _dt;
	auto M = _mass.asDiagonal();
	return 1.0 / _dt * M * (v - _v)
		+ _body_energy->EGradient(_reference, _volumn, _inv, x)
		+ _body_energy->DGradient(_reference, _volumn, _mass, _inv, _x, v);
}

MatrixXd BackwardTarget::Hessian(const VectorXd &x) const {
	VectorXd v = (x - _x) / _dt;
	auto M = _mass.asDiagonal().toDenseMatrix();
	return 1 / (_dt * _dt) * M
		+ _body_energy->EHessian(_reference, _volumn, _inv, x)
		+ _body_energy->DHessian(_reference, _volumn, _mass, _inv, _x, v);
}

DEFINE_CLONE(Function, BackwardTarget)