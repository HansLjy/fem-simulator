//
// Created by hansljy on 2022/2/23.
//

#include "BodyEnergy.h"

Matrix3d GetDs(const VectorXd& X, const std::array<int, 4>& tet) {
	Vector3d X1 = X.block<3, 1>(3 * tet[0], 0);	// TODO is this working?
	Vector3d X2 = X.block<3, 1>(3 * tet[1], 0);
	Vector3d X3 = X.block<3, 1>(3 * tet[2], 0);
	Vector3d X4 = X.block<3, 1>(3 * tet[3], 0);
	Matrix3d D;
	D.col(0) = X1 - X4;
	D.col(1) = X2 - X4;
	D.col(2) = X3 - X4;
	return D;
}

#define GET_MEMBERS(ref, tets, points, B)	\
	const auto& tets = ref.GetTets();		\
	const auto& points = ref.GetPoints(); 	\
	const auto& B = ref.GetB();

double
BodyEnergy::EEnergy(const ElasticEnergy &elas_model, const Mesh &reference,
					const VectorXd &X) {
	GET_MEMBERS(reference, tets, points, B)
	double energy = 0;
	int num_of_tets = tets.size();
	for (int i = 0; i < num_of_tets; i++) {
		auto D = GetDs(points, tets[i]);
		energy += elas_model.Energy(B[i], D);
	}
	return energy;
}

VectorXd
BodyEnergy::EGradient(const ElasticEnergy &elas_model, const Mesh &reference,
					  const VectorXd &X) {
	GET_MEMBERS(reference, tets, points, B)

	VectorXd gradient(X.size());
	gradient.setZero();

	int num_of_tets = tets.size();
	for (int i = 0; i < num_of_tets; i++) {
		auto tet = tets[i];
		auto D = GetDs(points, tet);
		Vector12d local_gradient = elas_model.Gradient(B[i], D);
		for (int j = 0; j < 4; j++) {
			gradient.block<3, 1>(3 * tet[j], 0) += local_gradient.block<3, 1>(3 * j, 0);
		}
	}

	return gradient;
}

MatrixXd
BodyEnergy::EHessian(const ElasticEnergy &elas_model, const Mesh &reference,
					 const VectorXd &X) {
	GET_MEMBERS(reference, tets, points, B)

	MatrixXd hessian(X.size(), X.size());
	hessian.setZero();

	int num_of_tets = tets.size();
	for (int i = 0; i < num_of_tets; i++) {
		auto tet = tets[i];
		auto D = GetDs(points, tet);
		Matrix12d local_hessian = elas_model.Hessian(B[i], D);
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				hessian.block<3, 3>(3 * tet[j], 3 * tet[k]) += local_hessian.block<3, 3>(3 * j, 3 * k);
			}
		}
	}

	return hessian;
}
