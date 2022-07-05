//
// Created by hansljy on 22-5-24.
//

#include "FixedSlab.h"

DEFINE_CLONE(Object, FixedSlab)

FixedSlab::FixedSlab(double mu, double rho, const Vector3d &center,
					 const Vector3d &euler_angles, const Vector3d &size) : RigidBody(mu, rho, center, euler_angles, Rectangle(size)) {
	_mass = COO();
	_x.resize(0);
	_v.resize(0);
}

SparseMatrixXd FixedSlab::GetJ(int idx, const Vector3d &point) const {
	return SparseMatrixXd(3, 0);
}