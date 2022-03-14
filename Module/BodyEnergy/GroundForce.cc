//
// Created by hansljy on 2022/3/14.
//

#include "GroundForce.h"

DEFINE_CLONE(ExternalForce, GroundForce)

GroundForce::GroundForce(double stiffness) : _stiffness(stiffness) {}

double
GroundForce::Energy(const Mesh &mesh, const VectorXd &mass, const VectorXd &X,
					const VectorXd &V) const {
	const auto& points = mesh.GetPoints();

	const int num_of_points = points.size() / 3;

	double result = 0;
	for (int i = 0; i < num_of_points; i++) {
		double z = points(3 * i + 2);
		if (z < 0) {
			result += z * z * _stiffness;
		}
	}

	return result;
}

VectorXd
GroundForce::Gradient(const Mesh &mesh, const VectorXd &mass, const VectorXd &X,
					  const VectorXd &V) const {
	const auto& points = mesh.GetPoints();

	const int num_of_points = points.size() / 3;

	VectorXd gradient(num_of_points * 3);
	gradient.setZero();
	for (int i = 0; i < num_of_points; i++) {
		double z = points(3 * i + 2);
		if (z < 0) {
			gradient(3 * i + 2) = 2 * z * _stiffness;
		}
	}
	return gradient;
}

MatrixXd
GroundForce::Hessian(const Mesh &mesh, const VectorXd &mass, const VectorXd &X,
					 const VectorXd &V) const {
	const auto& points = mesh.GetPoints();

	const int num_of_points = points.size() / 3;

	MatrixXd hessian(num_of_points * 3, num_of_points * 3);
	hessian.setZero();

	for (int i = 0; i < num_of_points; i++) {
		double z = points(3 * i + 2);
		if (z < 0) {
			hessian(3 * i + 2, 3 * i + 2) = 2 * _stiffness;
		}
	}
	return hessian;
}