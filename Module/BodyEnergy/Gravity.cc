//
// Created by hansljy on 2022/3/11.
//

#include "Gravity.h"

DEFINE_CLONE(ExternalForce, Gravity)

Gravity::Gravity(double g) : _g(g) {}

double
Gravity::Energy(const SoftBody &body) const {
	const auto& X = body._mesh.GetPoints();
	const int num_of_points = X.size() / 3;

	double energy = 0;
	for (int i = 0; i < num_of_points; i++) {
		energy += X(3 * i + 2) * body._mass(i) * _g;	// E = mgh
	}
	return energy;
}

VectorXd
Gravity::Gradient(const SoftBody &body) const {
	const auto& X = body._mesh.GetPoints();
	const int num_of_points = X.size() / 3;
	VectorXd gradient(3 * num_of_points);
	gradient.setZero();

	for (int i = 0; i < num_of_points; i++) {
		gradient(3 * i + 2) = body._mass(i) * _g;
	}

	return gradient;
}

SparseMatrixXd
Gravity::Hessian(const SoftBody &body) const {
	const auto& X = body._mesh.GetPoints();
	const int num_of_points = X.size() / 3;
	SparseMatrixXd hessian(3 * num_of_points, 3 * num_of_points);
	return hessian;
}
