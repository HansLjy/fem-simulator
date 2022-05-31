//
// Created by hansljy on 2022/4/9.
//

#ifndef FEM_RIGIDBODY_H
#define FEM_RIGIDBODY_H

#include "Util/EigenAll.h"
#include "Util/Pattern.h"
#include "Contact/Surface.h"
#include "Object/Object.h"
#include "Shape/Shape.h"
#include <string>
#include <spdlog/spdlog.h>

class RigidBodySurface;

class RigidBody : public Object {
public:
	RigidBody(double mu, double rho, const Vector3d& center, const Vector3d& euler_angles, const Shape &shape);

	int GetDOF() const override = 0;
	VectorXd & GetX() override {
		return _x;
	}
	const VectorXd & GetX() const override {
		return _x;
	}
	VectorXd & GetV() override {
		return _v;
	}
	const VectorXd & GetV() const override {
		return _v;
	}

	const COO& GetM() const override {
		return _mass;
	}

	/* Rigid bodies do not contain inner energy */
	double InternalEnergy() const override {
		return 0;
	}

	VectorXd InternalEnergyGradient() const override {
		VectorXd energy_gradient(GetDOF());
		energy_gradient.setZero();
		return energy_gradient;
	}

	COO InternalEnergyHessianCOO() const override {
		return COO();
	}

	double GetMu() const {
		return _mu;
	}

	void Store(const std::string &file) override {
		_shape->Store(file, GetRotation(), GetCenter());
	}

	const Surface* GetSurface() const override;

	COO GetJ(const SurfaceElements::SurfaceType &type, int idx,
			 const VectorXd &point,
			 const VectorXd &normal) const override = 0;

	RigidBody(const RigidBody& rhs);
	RigidBody& operator=(const RigidBody& rhs);
	virtual ~RigidBody() noexcept;

	virtual Matrix3d GetRotation() const = 0;
	virtual Vector3d GetCenter() const = 0;

	Object * Clone() const override = 0;

	friend RigidBodySurface;

protected:
	double _mu;
	double _rho;
	const Shape* _shape;
	const RigidBodySurface* _surface;

	Vector3d _center;
	Matrix3d _rotation;

	VectorXd _x;
	VectorXd _v;
	COO _mass;
};

class RigidBodySurface : public Surface {
public:
	RigidBodySurface(const RigidBody* rigid_body) : _rigid_body(rigid_body) {}

	int GetNumFaces() const override {
		return _rigid_body->_shape->GetNumFaces();
	}

	SurfaceElements::Face GetFace(int idx) const override {
		return _rigid_body->_shape->GetFace(idx, _rigid_body->GetRotation(), _rigid_body->GetCenter());
	}

protected:
	const RigidBody* _rigid_body;
};

#endif //FEM_RIGIDBODY_H
