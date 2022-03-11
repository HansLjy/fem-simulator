//
// Created by hansljy on 2022/2/23.
//

#include "StVKModel.h"
#include <spdlog/spdlog.h>

StVKModelParameter::StVKModelParameter(double youngs_module,
									   double poisson_ratio) : _youngs_module(youngs_module), _poisson_ratio(poisson_ratio) {}

DEFINE_CLONE(ConsistencyModelParameter, StVKModelParameter)
DEFINE_ACCESSIBLE_MEMBER(StVKModelParameter, double, YoungsModule, _youngs_module)
DEFINE_ACCESSIBLE_MEMBER(StVKModelParameter, double, PoissonRatio, _poisson_ratio)

void StVKModel::Initialize(const ConsistencyModelParameter& para) {
	_lame_mu = para.GetYoungsModule() / (2 * (1 + para.GetPoissonRatio()));
	_lame_lambda = para.GetYoungsModule() * para.GetPoissonRatio() / ((1 + para.GetPoissonRatio()) * (1 - 2 * para.GetPoissonRatio()));
	spdlog::info("StVKModel initialized");
}

double StVKModel::EnergyDensity(const Matrix3d &F) const {
	auto E = 0.5 * (F.transpose() * F - Matrix3d::Identity());
	return _lame_mu * (E.transpose() * E).trace() + _lame_lambda / 2.0 * E.trace() * E.trace();
}

Matrix3d StVKModel::Piola(const Matrix3d &F) const {
	auto E = 0.5 * (F.transpose() * F - Matrix3d::Identity());
	return F * (2 * _lame_mu * E + _lame_lambda * E.trace() * Matrix3d::Identity());
}

Matrix3d StVKModel::PiolaDifferential(const Matrix3d &F,
									  const Matrix3d &dF) const {
	auto E = 0.5 * (F.transpose() * F - Matrix3d::Identity());
	auto dE = 0.5 * (F.transpose() * dF + dF.transpose() * F);
	return 2 * _lame_mu * dF * E + _lame_lambda * E.trace() * dF + 2 * _lame_mu * F * dE + _lame_lambda * dE.trace() * F;
}

StVKModel::~StVKModel() = default;

DEFINE_CLONE(ConsistencyModel, StVKModel)