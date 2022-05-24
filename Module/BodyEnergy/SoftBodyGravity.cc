//
// Created by hansljy on 22-5-24.
//

#include "SoftBodyGravity.h"

DEFINE_CLONE(ExternalForce, SoftBodyGravity)

SoftBodyGravity::SoftBodyGravity(const SoftBody &soft_body, double g) : _soft_body(&soft_body), _g(g) {}

double SoftBodyGravity::Energy() const {
	double energy = 0;
	const int num_nodes = _soft_body->GetDOF() / 3;
	const auto& coord = _soft_body->GetX();
	for (int i = 0; i < num_nodes; i++) {
		energy += _soft_body->_mass[i] * _g * coord[3 * i];
	}
	return energy;
}

VectorXd SoftBodyGravity::Gradient() const {
	VectorXd gradient(_soft_body->GetDOF());
	gradient.setZero();
	const int num_nodes = _soft_body->GetDOF() / 3;
	for (int i = 0; i < num_nodes; i++) {
		gradient[3 * i] = _soft_body->_mass[i] * _g;
	}
	return gradient;
}

SparseMatrixXd SoftBodyGravity::Hessian() const {
	return SparseMatrixXd(_soft_body->GetDOF(), _soft_body->GetDOF());
}