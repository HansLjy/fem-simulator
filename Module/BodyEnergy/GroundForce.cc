//
// Created by hansljy on 2022/3/14.
//

#include "GroundForce.h"
#include "Util/EigenAll.h"
#include "SoftBody/SoftBody.h"

DEFINE_CLONE(ExternalForce, GroundForce)

GroundForce::GroundForce(double stiffness) : _stiffness(stiffness) {}

double
GroundForce::Energy(const SoftBody &body) const {
	const auto& X = body._mesh.GetPoints();
	const int num_of_points = X.size() / 3;

	double result = 0;
	for (int i = 0; i < num_of_points; i++) {
		double z = X(3 * i + 2);
		if (z < 0) {
			result += z * z * _stiffness;
		}
	}

	return result;
}

VectorXd
GroundForce::Gradient(const SoftBody &body) const {
	const auto& X = body._mesh.GetPoints();
	const int num_of_points = X.size() / 3;

	VectorXd gradient(num_of_points * 3);
	gradient.setZero();
	for (int i = 0; i < num_of_points; i++) {
		double z = X(3 * i + 2);
		if (z < 0) {
			gradient(3 * i + 2) = 2 * z * _stiffness;
		}
	}
	return gradient;
}

SparseMatrixXd
GroundForce::Hessian(const SoftBody &body) const {
	const auto& X = body._mesh.GetPoints();
	const int num_of_points = X.size() / 3;

	SparseMatrixXd hessian(num_of_points * 3, num_of_points * 3);
	std::vector<Triplet> triplets;

	for (int i = 0; i < num_of_points; i++) {
		double z = X(3 * i + 2);
		if (z < 0) {
			triplets.push_back(Triplet(3 * i + 2, 3 * i + 2, 2 * _stiffness));
		}
	}
	hessian.setFromTriplets(triplets.begin(), triplets.end());
	return hessian;
}