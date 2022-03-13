//
// Created by hansljy on 2022/3/11.
//

#include "Gravity.h"

DEFINE_CLONE(ExternalForce, Gravity)

Gravity::Gravity(double g) : _g(g) {}

double
Gravity::Energy(const Mesh &mesh, const VectorXd &mass, const VectorXd &X,
				const VectorXd &V) const {
	const int num_of_points = mesh.GetPoints().size() / 3;

	double energy = 0;
	for (int i = 0; i < num_of_points; i++) {
		energy += X(3 * i + 2) * mass(i) * _g;	// E = mgh
	}
	return energy;
}

VectorXd
Gravity::Gradient(const Mesh &mesh, const VectorXd &mass, const VectorXd &X,
				  const VectorXd &V) const {
	const int num_of_points = mesh.GetPoints().size() / 3;
	VectorXd gradient(3 * num_of_points);
	gradient.setZero();

	for (int i = 0; i < num_of_points; i++) {
		gradient(3 * i + 2) = mass(i) * _g;
	}

	return gradient;
}

MatrixXd
Gravity::Hessian(const Mesh &mesh, const VectorXd &mass, const VectorXd &X,
				 const VectorXd &V) const {
	const int num_of_points = mesh.GetPoints().size() / 3;
	MatrixXd hessian(3 * num_of_points, 3 * num_of_points);
	hessian.setZero();
	return hessian;
}
