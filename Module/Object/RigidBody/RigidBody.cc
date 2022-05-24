//
// Created by hansljy on 22-5-17.
//

#include "RigidBody.h"

RigidBody::RigidBody(double mu, double rho, const Vector3d &center,
					 const Vector3d &euler_angles, const Shape *shape)
					 : _mu(mu), _rho(rho), _center(center), _shape(shape) {
	_rotation = AngleAxisd(euler_angles(0), Vector3d::UnitZ())
				* AngleAxisd(euler_angles(1), Vector3d::UnitX())
				* AngleAxisd(euler_angles(2), Vector3d::UnitZ());
}

const Surface *RigidBody::GetSurface() const {
	return _surface;
}

RigidBody::RigidBody(const RigidBody &rhs) {
	_mu = rhs._mu;
	_rho = rhs._rho;
	_shape = rhs._shape->Clone();
	_surface = new RigidBodySurface(this);
	_mass = rhs._mass;
}