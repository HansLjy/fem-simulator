//
// Created by hansljy on 2022/3/7.
//

#include "../Test.h"
#include "ElementEnergy/SimpleModel.h"
#include "ConsistencyModel/StVKModel.h"
#include "Util/Factory.h"
#include <array>
typedef std::array<Vector3d, 4> Tet;

void Test::TestElasticForce() {
	spdlog::info("Start testing elastic energy model");
	Tet tet;

	tet[0] << 1, 0, 0;
	tet[1] << 0, 1, 0;
	tet[2] << 0, 0, 1;
	tet[3] << 0, 0, 0;

	Matrix3d D;
	for (int i = 0; i < 3; i++) {
		D.col(i) = tet[i] - tet[3];
	}

	double W = D.determinant() / 6;

	Matrix3d B = D.inverse();

	// Stay put
	auto energy = _elas_model->Energy(*_consistency_model, W, B, D);
	auto gradient = _elas_model->Gradient(*_consistency_model, W, B, D);
	auto hessian = _elas_model->Hessian(*_consistency_model, W, B, D);

	double error_hessian = 0;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 12; j++) {
			error_hessian += hessian(j, i) + hessian(j, i + 3) + hessian(j, i + 6) + hessian(j, i + 9);
			error_hessian += hessian(i, j) + hessian(i + 3, j) + hessian(i + 6, j) + hessian(i + 9, j);
		}
	}

	CPPUNIT_ASSERT(0 <= energy && energy < _eps);
	CPPUNIT_ASSERT(gradient.norm() < _eps);
	CPPUNIT_ASSERT(std::abs(error_hessian) < _eps);

	// Rotate around the corner
	for (int i = 0; i < 3; i++) {
		D.col(i) = tet[(i + 1) % 3] - tet[3];
	}
	energy = _elas_model->Energy(*_consistency_model, W, B, D);
	gradient = _elas_model->Gradient(*_consistency_model, W, B, D);
	hessian = _elas_model->Hessian(*_consistency_model, W, B, D);

	error_hessian = 0;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 12; j++) {
			error_hessian += hessian(j, i) + hessian(j, i + 3) + hessian(j, i + 6) + hessian(j, i + 9);
			error_hessian += hessian(i, j) + hessian(i + 3, j) + hessian(i + 6, j) + hessian(i + 9, j);
		}
	}

	CPPUNIT_ASSERT(0 <= energy && energy < _eps);
	CPPUNIT_ASSERT(gradient.norm() < _eps);
	CPPUNIT_ASSERT(std::abs(error_hessian) < _eps);

	// Check using numerical differentiation

	const double h = 0.01;	// single step

	for (int T = 0; T < 10; T++) {

		for (int i = 0; i < 4; i++) {
			tet[i] = Vector3d::Random();
		}
		for (int i = 0; i < 3; i++) {
			D.col(i) = tet[i] - tet[3];
		}

		Vector12d num_gradient;    // numerical result for gradient
		Matrix12d num_hessian;    // numerical result for hessian

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				double tmp = D(i, j);
				D(i, j) = tmp + h;
				double f_plus = _elas_model->Energy(*_consistency_model, W, B, D);
				D(i, j) = tmp - h;
				double f_minus = _elas_model->Energy(*_consistency_model, W, B, D);
				num_gradient(3 * j + i) = (f_plus - f_minus) / (2 * h);
				D(i, j) = tmp;
			}
		}

		for (int i = 0; i < 3; i++) {
			Vector3d tmp = D.row(i);
			D.row(i) = tmp - h * Vector3d::Ones();
			double f_plus = _elas_model->Energy(*_consistency_model, W, B, D);
			D.row(i) = tmp + h * Vector3d::Ones();
			double f_minus = _elas_model->Energy(*_consistency_model, W, B, D);
			num_gradient(9 + i) = (f_plus - f_minus) / (2 * h);
			D.row(i) = tmp;
		}

		for (int i1 = 0; i1 < 3; i1++) {
			for (int j1 = 0; j1 < 3; j1++) {
				for (int i2 = 0; i2 < 3; i2++) {
					for (int j2 = 0; j2 < 3; j2++) {
						if (i1 != i2 || j1 != j2) {
							double tmp1 = D(i1, j1), tmp2 = D(i2, j2);
							double f[2][2];
							for (int delta1: {-1, 1}) {
								for (int delta2: {-1, 1}) {
									D(i1, j1) = tmp1 + delta1 * h;
									D(i2, j2) = tmp2 + delta2 * h;
									f[(delta1 + 1) >> 1][(delta2 + 1)
											>> 1] = _elas_model->Energy(
											*_consistency_model, W, B, D);
								}
							}
							D(i1, j1) = tmp1;
							D(i2, j2) = tmp2;
							num_hessian(j1 * 3 + i1, j2 * 3 + i2) =
									((f[1][1] - f[1][0]) -
									 (f[0][1] - f[0][0])) / (4 * h * h);
						} else {
							int i = i1, j = j1;
							double tmp = D(i, j);
							double f[5];
							for (int delta = -2; delta <= 2; delta++) {
								D(i, j) = tmp + delta * h;
								f[delta + 2] = _elas_model->Energy(*_consistency_model,
																   W, B, D);
							}
							D(i, j) = tmp;
							num_hessian(j * 3 + i, j * 3 + i) =
									(-f[4] + 16 * f[3] - 30 * f[2] + 16 * f[1] -
									 f[0]) / (12 * h * h);
						}
					}
				}
			}
		}

		for (int i = 0; i < 3; i++) {
			num_hessian.col(i + 9) =
					-num_hessian.col(i) - num_hessian.col(i + 3) -
					num_hessian.col(i + 6);
		}
		for (int i = 0; i < 3; i++) {
			num_hessian.row(i + 9) =
					-num_hessian.row(i) - num_hessian.row(i + 3) -
					num_hessian.row(i + 6);
		}

		gradient = _elas_model->Gradient(*_consistency_model, W, B, D);
		hessian = _elas_model->Hessian(*_consistency_model, W, B, D);

		CPPUNIT_ASSERT((gradient - num_gradient).norm() < 0.01);
		CPPUNIT_ASSERT((hessian - num_hessian).norm() < 0.01);
	}

	spdlog::info("Finish testing elastic energy model");
}