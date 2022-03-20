//
// Created by hansljy on 2022/2/23.
//

#include "SimpleModel.h"
#include <spdlog/spdlog.h>

DEFINE_CLONE(ElasticEnergyModelParameter, SimpleModelParameter)

void SimpleModel::Initialize(const ElasticEnergyModelParameter &para) {
	spdlog::info("SimpleModel initialized");
}

double SimpleModel::Energy(const ConsistencyModel &cons_model, double W,
						   const Matrix3d &B,
						   const Matrix3d &Ds) const {
	Matrix3d F = Ds * B;
	return W * cons_model.EnergyDensity(F);
}

Vector12d SimpleModel::Gradient(const ConsistencyModel &cons_model, double W,
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

Matrix12d SimpleModel::Hessian(const ConsistencyModel &cons_model, double W,
							   const Matrix3d &B,
							   const Matrix3d &Ds) const {
	auto start = clock();
	Matrix3d F = Ds * B;
	Matrix3d I3 = Matrix3d::Identity();
	Matrix12d hessian;

	Eigen::Matrix<double, 12, 9> pdFX;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			pdFX.block<3, 3>(3 * i, 3 * j) = B(i, j) * I3;
		}
	}
	for (int i = 0; i < 3; i++) {
		pdFX.row(i + 9) = - pdFX.row(i) - pdFX.row(i + 3) - pdFX.row(i + 6);
	}

	Matrix9d pdPsiF2 = cons_model.PiolaDifferential(F);

	hessian = W * pdFX * pdPsiF2 * pdFX.transpose();
//	spdlog::info("Time for computing a single hessian matrix: {}", clock() - start);
	return hessian;
}

SimpleModel::~SimpleModel() = default;

DEFINE_CLONE(ElasticEnergyModel, SimpleModel)