//
// Created by hansljy on 2022/2/23.
//

#include "SimpleModel.h"
#include <spdlog/spdlog.h>

DEFINE_CLONE(ElasticEnergyModelParameter, SimpleModelParameter)

void SimpleModel::Initialize(const ElasticEnergyModelParameter &para) {
	spdlog::info("SimpleModel initialized");
}

double SimpleModel::Energy(const ConstituteModel &cons_model, double W,
						   const Matrix3d &B,
						   const Matrix3d &Ds) const {
	Matrix3d F = Ds * B;
	return W * cons_model.EnergyDensity(F);
}

Vector12d SimpleModel::Gradient(const ConstituteModel &cons_model, double W,
								const Matrix3d &B,
								const Matrix3d &Ds) const {
	Matrix3d F = Ds * B;
	Matrix3d H = -W * cons_model.Piola(F) * B.transpose();
	Vector12d gradient;
	for (int i = 0; i < 3; i++) {
		gradient.block<3, 1>(3 * i, 0) = H.col(i);
	}
	gradient.block<3, 1>(9, 0) = - H.col(0) - H.col(1) - H.col(2);
	return -gradient;
}

Matrix12d SimpleModel::Hessian(const ConstituteModel &cons_model, double W,
							   const Matrix3d &B,
							   const Matrix3d &Ds,
							   const Matrix12x9d &pFpX) const {
//	auto start = clock();
	Matrix3d F = Ds * B;
	Matrix9d pdPsiF2 = cons_model.PiolaDifferential(F);
	Matrix12d hessian = W * pFpX * pdPsiF2 * pFpX.transpose();
//	spdlog::info("Time for computing a single hessian matrix: {} s", (double)(clock() - start) / CLOCKS_PER_SEC);
	return hessian;
}

SimpleModel::~SimpleModel() = default;

DEFINE_CLONE(ElasticEnergyModel, SimpleModel)