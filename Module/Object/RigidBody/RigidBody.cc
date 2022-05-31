//
// Created by hansljy on 22-5-17.
//

#include "RigidBody.h"
#include "Contact/Surface.h"

RigidBody::RigidBody(double mu, double rho, const Vector3d &center,
					 const Vector3d &euler_angles, const Shape &shape)
					 : _mu(mu), _rho(rho), _center(center), _shape(shape.Clone()), _surface(new RigidBodySurface(this)) {
	_rotation = AngleAxisd(euler_angles(0), Vector3d::UnitZ())
				* AngleAxisd(euler_angles(1), Vector3d::UnitX())
				* AngleAxisd(euler_angles(2), Vector3d::UnitZ());
}

const Surface *RigidBody::GetSurface() const {
	return _surface;
}

RigidBody::~RigidBody() noexcept {
	delete _surface;
	delete _shape;
}

RigidBody::RigidBody(const RigidBody &rhs) : Object(rhs) {
	_mu = rhs._mu;
	_rho = rhs._rho;
	_shape = rhs._shape->Clone();
	_surface = new RigidBodySurface(this);
	_center = rhs._center;
	_rotation = rhs._rotation;
	_x = rhs._x;
	_v = rhs._v;
	_mass = rhs._mass;
}

RigidBody &RigidBody::operator=(const RigidBody &rhs) {
	if (this == &rhs) {
		return *this;
	}
	Object::operator=(rhs);
	_mu = rhs._mu;
	_rho = rhs._rho;
	delete _shape;
	_shape = rhs._shape->Clone();
	delete _surface;
	_surface = new RigidBodySurface(this);
	_center = rhs._center;
	_rotation = rhs._rotation;
	_x = rhs._x;
	_v = rhs._v;
	_mass = rhs._mass;
	return *this;
}
