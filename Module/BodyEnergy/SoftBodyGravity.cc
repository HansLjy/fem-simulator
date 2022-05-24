//
// Created by hansljy on 22-5-24.
//

#include "SoftBodyGravity.h"
#include "Object/SoftBody/SoftBody.h"

DEFINE_CLONE(ExternalForce, SoftBodyGravity)

SoftBodyGravity::SoftBodyGravity(double g) : _g(g) {}

double SoftBodyGravity::Energy(const Object &obj) const {
	auto& soft_obj = dynamic_cast<const SoftBody&>(obj);
	double energy = 0;
	const int num_nodes = soft_obj.GetDOF() / 3;
	const auto& coord = soft_obj.GetX();
	for (int i = 0; i < num_nodes; i++) {
		energy += soft_obj._mass[i] * _g * coord[3 * i];
	}
	return energy;
}

VectorXd SoftBodyGravity::Gradient(const Object &obj) const {
	auto& soft_obj = dynamic_cast<const SoftBody&>(obj);
	VectorXd gradient(soft_obj.GetDOF());
	gradient.setZero();
	const int num_nodes = soft_obj.GetDOF() / 3;
	for (int i = 0; i < num_nodes; i++) {
		gradient[3 * i] = soft_obj._mass[i] * _g;
	}
	return gradient;
}

SparseMatrixXd SoftBodyGravity::Hessian(const Object &obj) const {
	return SparseMatrixXd(obj.GetDOF(), obj.GetDOF());
}