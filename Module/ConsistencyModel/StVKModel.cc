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

Matrix9d StVKModel::PiolaDifferential(const Matrix3d &F) const {
	Matrix3d FT = F.transpose();
	Matrix3d FTF = FT * F;
	Matrix3d FFT = F * FT;
	double trE = 0.5 * (FTF.trace() - 3);
	Matrix3d I3 = Matrix3d::Identity();
	Matrix9d result;
	result.setZero();
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			result.block<3, 3>(3 * i, 3 * j) += _lame_mu * FTF(i, j) * I3 + 4 * _lame_lambda * F.col(i) * F.col(j).transpose();
		}
		result.block<3, 3>(3 * i, 3 * i) += _lame_mu * FFT + (-_lame_mu + 2 * _lame_lambda * trE) * I3;
	}
//	for (int i = 0; i < 3; i++) {
//		for (int j = i + 1; j < 3; j++) {
//			result.block<3, 3>(3 * i, 3 * j) = result.block<3, 3>(3 * j, 3 * i).transpose();
//		}
//	}
	Matrix9d FkronFT, FkronFT_permed;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			FkronFT.block<3, 3>(3 * i, 3 * j) = _lame_mu * F(i, j) * FT;
		}
	}

	static const int K33[] = {0, 3, 6, 1, 4, 7, 2, 5, 8};

	for (int i = 0; i < 9; i++) {
		FkronFT_permed.row(i) = FkronFT.row(K33[i]);
	}

	assert((FkronFT_permed.transpose() - FkronFT_permed).norm() < 1e-5);

	result += FkronFT_permed;
	return result;
}

StVKModel::~StVKModel() = default;

DEFINE_CLONE(ConsistencyModel, StVKModel)