//
// Created by hansljy on 2022/2/23.
//

#include "SimpleModel.h"
#include <spdlog/spdlog.h>

/**
 * Project a matrix into a positive definite one
 */

template<int dim>
Matrix<double, dim, dim> PositiveProject(const Eigen::Matrix<double, dim, dim>& matrix) {
	typedef Eigen::Matrix<double, dim, dim> MatrixOfSize;
	typedef Eigen::Vector<double, dim> VectorOfSize;
	Eigen::SelfAdjointEigenSolver<MatrixOfSize> eigens(matrix);
	MatrixOfSize eigen_vectors = eigens.eigenvectors();
	VectorOfSize eigen_values = eigens.eigenvalues();
	for (int i = 0; i < dim; i++) {
		if (eigen_values[i] < 0) {
			eigen_values[i] = 0;
		}
	}
	return eigen_vectors * eigen_values.asDiagonal() * eigen_vectors.transpose();
}

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
	Matrix9d pdPsiF2 = PositiveProject(cons_model.PiolaDifferential(F));
	Matrix12d hessian = W * pFpX * pdPsiF2 * pFpX.transpose();
//	spdlog::info("Time for computing a single hessian matrix: {} s", (double)(clock() - start) / CLOCKS_PER_SEC);
	return hessian;
}

SimpleModel::~SimpleModel() = default;

DEFINE_CLONE(ElasticEnergyModel, SimpleModel)