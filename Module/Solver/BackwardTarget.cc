//
// Created by hansljy on 2022/2/22.
//

#include "BackwardTarget.h"
#include "BodyEnergy/BodyEnergy.h"
#include "Eigen/Sparse"

double BackwardTarget::Value(const VectorXd &x) {
	VectorXd v = (x - _x) / _dt;
	auto M = _reference.GetMass().asDiagonal();
	return 0.5 * (v - _v).transpose() * M * (v - _v)
		+ BodyEnergy::EEnergy(*_elas_model, _reference, x)
		+ BodyEnergy::DEnergy(*_diss_model, _reference, _x, v);
}


VectorXd BackwardTarget::Gradient(const VectorXd &x) {
	VectorXd v = (x - _x) / _dt;
	auto M = _reference.GetMass().asDiagonal();
	return 1.0 / _dt * M * (v - _v)
		+ BodyEnergy::EGradient(*_elas_model, _reference, x)
		+ BodyEnergy::DGradient(*_diss_model, _reference, _x, v);
}

MatrixXd BackwardTarget::Hessian(const VectorXd &x) {
	VectorXd v = (x - _x) / _dt;
	auto M = _reference.GetMass().asDiagonal().toDenseMatrix();
	return 1 / (_dt * _dt) * M
		+ BodyEnergy::EHessian(*_elas_model, _reference, x)
		+ BodyEnergy::DHessian(*_diss_model, _reference, _x, v);
}