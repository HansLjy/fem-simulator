//
// Created by hansljy on 2022/2/23.
//

#include "StVKModel.h"

double StVKModel::EnergyDensity(const Matrix3d &F) {
	auto E = 0.5 * (F.transpose() * F - Matrix3d::Identity());
	return _lame_mu * (E.transpose() * E).trace() + _lame_lambda / 2.0 * E.trace() * E.trace();
}

Matrix3d StVKModel::Piola(const Matrix3d &F) {
	auto E = 0.5 * (F.transpose() * F - Matrix3d::Identity());
	return F * (2 * _lame_mu * E + _lame_lambda * E.trace() * Matrix3d::Identity());
}

Matrix3d StVKModel::PiolaDifferential(const Matrix3d &F, const Matrix3d &dF) {
	auto E = 0.5 * (F.transpose() * F - Matrix3d::Identity());
	auto dE = 0.5 * (F.transpose() * dF + dF.transpose() * F);
	return 2 * _lame_mu * dF * E + _lame_lambda * E.trace() * dF + 2 * _lame_mu * F * dE + _lame_lambda * dE.trace() * F;
}