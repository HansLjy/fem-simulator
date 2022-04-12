//
// Created by hansljy on 2022/3/7.
//

#include "../Test.h"
#include "ConstituteModel/StVKModel.h"

void Test::TestConstituteModel() {
	spdlog::info("Start Constitute Model Resource");

	//<- Identity matrix, psi, piola and piola differential should all be 0
	Matrix3d I = Matrix3d::Identity();
	auto psi = _consistency_model->EnergyDensity(I);
	auto piola = _consistency_model->Piola(I);
//	auto piola_diff = _consistency_model->PiolaDifferential(I);

	CPPUNIT_ASSERT(psi < _eps && psi > -_eps);
	CPPUNIT_ASSERT(piola.norm() < _eps && piola.norm() > -_eps);
//	CPPUNIT_ASSERT(piola_diff.norm() < _eps && piola_diff.norm() > -_eps);

	// <- Resource a random rotational matrix

	Vector3d pivot = Vector3d::Random().normalized();
	double x = pivot.x(), y = pivot.y(), z = pivot.z();
	double theta = (double)rand() / (double)RAND_MAX * 2 * acos(-1);
	double cos_theta = cos(theta);
	double sin_theta = sin(theta);

	Matrix3d R;	// R is supposed to be a rotation matrix
	R << cos_theta + (1 - cos_theta) * x * x, (1 - cos_theta) * x * y - sin_theta * z, (1 - cos_theta) * x * z + sin_theta * y,
	     (1 - cos_theta) * x * y + sin_theta * z, cos_theta + (1 - cos_theta) * y * y, (1 - cos_theta) * y * z - sin_theta * x,
		 (1 - cos_theta) * x * z - sin_theta * y, (1 - cos_theta) * z * y + sin_theta * x, cos_theta + (1 - cos_theta) * z * z;

	assert((R.transpose() * R - Matrix3d::Identity()).norm() < _eps);

	psi = _consistency_model->EnergyDensity(R);
	piola = _consistency_model->Piola(R);
//	piola_diff = _consistency_model->PiolaDifferential(R);
	CPPUNIT_ASSERT(psi < _eps && -_eps < psi);
	CPPUNIT_ASSERT(piola.norm() < _eps && piola.norm() > -_eps);
//	CPPUNIT_ASSERT(piola_diff.norm() < _eps && piola_diff.norm() > -_eps);

	// <- This is an example calculated by hand

	Matrix3d F, dF, P, dP;
	F << 1, 2, 3,
		 4, 5, 6,
		 7, 8, 9;

	dF << -1, 1, 1,
	      1, -1, 1,
		  1, 1, -1;

	P << 52.2500, 5.5000, -41.2500,
		 15.5000, 2.5000, -10.5000,
		 -21.2500,-0.5000, 20.2500;

	dP << 108.7500, -42.5000, -52.2500,
		  -27.5000, 91.5000, -72.5000,
		  -22.2500, -57.5000, 48.7500;


	psi = _consistency_model->EnergyDensity(F);
	piola = _consistency_model->Piola(F);
//	piola_diff = _consistency_model->PiolaDifferential(F);

	CPPUNIT_ASSERT(std::abs(psi - 30.375) < _eps);
	CPPUNIT_ASSERT((P - piola).norm() < _eps);
//	CPPUNIT_ASSERT((dP - piola_diff).norm() < _eps);

	spdlog::info("Finish Constitute Model Resource");
}