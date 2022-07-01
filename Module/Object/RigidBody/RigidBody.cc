//
// Created by hansljy on 22-5-17.
//

#include "RigidBody.h"
#include "Object/DOFShapeConverter.h"
#include "RigidBodyDOFShapeConverter.h"

RigidBody::RigidBody(double mu, double rho, const Vector3d &center,
					 const Vector3d &euler_angles, const Shape &shape)
					 : _mu(mu), _rho(rho), _center(center), _shape(shape.Clone()) {
	_rotation = AngleAxisd(euler_angles(0), Vector3d::UnitZ())
				* AngleAxisd(euler_angles(1), Vector3d::UnitX())
				* AngleAxisd(euler_angles(2), Vector3d::UnitZ());
}

const DOFShapeConverter *RigidBody::GetDOFShapeConverter() const {
	return _DOF_converter;
}

RigidBody::~RigidBody() noexcept {
	delete _DOF_converter;
	delete _shape;
}

RigidBody::RigidBody(const RigidBody &rhs) : Object(rhs) {
	_mu = rhs._mu;
	_rho = rhs._rho;
	_shape = rhs._shape->Clone();
	_DOF_converter = rhs._DOF_converter->Clone();
	_center = rhs._center;
	_rotation = rhs._rotation;
	_x = rhs._x;
	_v = rhs._v;
	_mass = rhs._mass;
}


void RigidBody::Store(const std::string &file) {
	_DOF_converter->Store(*this, file, OutputFormatType::kVtk);
}