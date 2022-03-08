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
	auto cons_model = ConsistencyModelFactory::GetInstance()->GetConsistencyModel(ConsistencyModelType::kStVK);
	cons_model->Initialize(StVKModelParameter(1, 1));
	Tet tet;

	tet[0] << 1, 0, 0;
	tet[1] << 0, 1, 0;
	tet[2] << 0, 0, 1;
	tet[3] << 0, 0, 0;

	Matrix3d D;
	for (int i = 0; i < 3; i++) {
		D.col(i) = tet[i] - tet[3];
	}

	Matrix3d B = D.inverse();
	_elas_model->Initialize(ElasticEnergyModelParameter());

	// Stay put
	auto energy = _elas_model->Energy(*cons_model, B, D);
	auto gradient = _elas_model->Gradient(*cons_model, B, D);
	auto hessian = _elas_model->Hessian(*cons_model, B, D);

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
	energy = _elas_model->Energy(*cons_model, B, D);
	gradient = _elas_model->Gradient(*cons_model, B, D);
	hessian = _elas_model->Hessian(*cons_model, B, D);

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

	const double h = 1e-6;	// single step
	tet[0] << 1, 0, 1;
	tet[1] << 1, 1, 0;
	tet[2] << 0, 1, 1;
	tet[3] << 0, 0, 0;
//	for (int i = 0; i < 4; i++) {
//		tet[i] = Vector3d::Random();
//	}
	for (int i = 0; i < 3; i++) {
		D.col(i) = tet[i] - tet[3];
	}

	Vector12d num_gradient;	// numerical result for gradient
	Matrix12d num_hessian;	// numerical result for hessian

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			double tmp = D(i, j);
			D(i, j) = tmp + h;
			double f_plus = _elas_model->Energy(*_consistency_model, B, D);
			D(i, j) = tmp - h;
			double f_minus = _elas_model->Energy(*_consistency_model, B, D);
			num_gradient(3 * j + i) = (f_plus - f_minus) / (2 * h);
			D(i, j) = tmp;
		}
	}

	for (int i = 0; i < 3; i++) {
		Vector3d tmp = D.row(i);
		D.row(i) = tmp + h * Vector3d::Ones();
		double f_plus = _elas_model->Energy(*_consistency_model, B, D);
		D.row(i) = tmp - h * Vector3d::Ones();
		double f_minus = _elas_model->Energy(*_consistency_model, B, D);
		num_gradient(9 + i) = (f_plus - f_minus) / (2 * h);
		D.row(i) = tmp;
	}

	gradient = _elas_model->Gradient(*_consistency_model, B, D);

	spdlog::info("Gradient: ");
	std::cerr << gradient << std::endl;
	spdlog::info("Numerical result for gradient: ");
	std::cerr << num_gradient << std::endl;

	CPPUNIT_ASSERT((gradient - num_gradient).norm() < _eps);

	spdlog::info("Finish testing elastic energy model");
}