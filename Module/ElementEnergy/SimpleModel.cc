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
	Matrix12d hessian;
	hessian.setZero();
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			Matrix3d dDs;
			dDs.setZero();
			dDs(i, j) = 1;
			Matrix3d dF = dDs * B;
			Matrix3d dH = - W * cons_model.PiolaDifferential(F, dF) * B.transpose();
			int col_id = j * 3 + i;
			for (int k = 0; k < 3; k++) {
				hessian.col(col_id).block<3, 1>(3 * k, 0) = dH.col(k);
			}
		}
	}
	for (int i = 0; i < 3; i++) {
		hessian.row(i + 9) = - hessian.row(i) - hessian.row(i + 3) - hessian.row(i + 6);
	}
	for (int i = 0; i < 3; i++) {
		hessian.col(i + 9) = - hessian.col(i) - hessian.col(i + 3) - hessian.col(i + 6);
	}
	spdlog::info("Time for computing a single hessian matrix: {}", clock() - start);
//	exit(0);
	return -hessian;
}

SimpleModel::~SimpleModel() = default;

DEFINE_CLONE(ElasticEnergyModel, SimpleModel)