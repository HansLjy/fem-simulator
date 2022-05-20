//
// Created by hansljy on 22-5-17.
//

#include "RigidBody.h"

RigidBody::RigidBody(double mu, double rho, const Shape *shape)
	: _mu(mu), _rho(rho), _shape(shape), _surface(new RigidBodySurface(this)) {}

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