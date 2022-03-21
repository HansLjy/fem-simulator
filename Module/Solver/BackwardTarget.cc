//
// Created by hansljy on 2022/2/22.
//

#include "BackwardTarget.h"
#include "BodyEnergy/BodyEnergy.h"
#include <spdlog/spdlog.h>
#include <ctime>

double BackwardTarget::Value(const VectorXd &x) const {
	VectorXd v = (x - _x) / _dt;
	auto M = _mass_sparse.asDiagonal();
	double result = 0.5 * (v - _v).transpose() * M * (v - _v)
					+ _body_energy->EEnergy(_reference, _volumn, _inv, x, _pFpX)
					+ _body_energy->DEnergy(_reference, _volumn, _mass, _inv,
											_x, v, _pFpX);

	for (const auto& force : _ext_force) {
		result += force->Energy(_reference, _mass, x, v);
	}
	return result;
}


VectorXd BackwardTarget::Gradient(const VectorXd &x) const {
	VectorXd v = (x - _x) / _dt;
	auto M = _mass_sparse.asDiagonal();
	VectorXd result =  1.0 / _dt * M * (v - _v)
		+ _body_energy->EGradient(_reference, _volumn, _inv, x, _pFpX)
		+ _body_energy->DGradient(_reference, _volumn, _mass, _inv, _x, v, _pFpX);

	for (const auto& force : _ext_force) {
		result += force->Gradient(_reference, _mass, x, v);
	}
	return result;
}

SparseMatrixXd BackwardTarget::Hessian(const VectorXd &x) const {
	VectorXd v = (x - _x) / _dt;
	SparseMatrixXd result = _body_energy->EHessian(_reference, _volumn, _inv, x, _pFpX)
		+ _body_energy->DHessian(_reference, _volumn, _mass, _inv, _x, v, _pFpX);
	result.diagonal() += _mass_sparse / (_dt * _dt);

	for (const auto& force : _ext_force) {
		result += force->Hessian(_reference, _mass, x, v);
	}
	return result;
}

DEFINE_CLONE(Function, BackwardTarget)