//
// Created by hansljy on 2022/4/9.
//

#ifndef FEM_RIGIDBODY_H
#define FEM_RIGIDBODY_H

#include "Util/EigenAll.h"
#include "Util/Pattern.h"
#include "Object/DOFShapeConverter.h"
#include "Object/Object.h"
#include "Shape/Shape.h"
#include <string>
#include <spdlog/spdlog.h>

class RigidBodyDOFShapeConverter;

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

	void Store(const std::string &file) override;

	const DOFShapeConverter* GetDOFShapeConverter() const override;

	RigidBody(const RigidBody& rhs);
	virtual ~RigidBody() noexcept;

	virtual Matrix3d GetRotation() const = 0;
	virtual Vector3d GetCenter() const = 0;

	Object * Clone() const override = 0;

	friend RigidBodyDOFShapeConverter;

protected:
	double _mu;
	double _rho;
	const Shape* _shape;
	DOFShapeConverter *_DOF_converter;

	Vector3d _center;
	Matrix3d _rotation;

	VectorXd _x;
	VectorXd _v;
	COO _mass;
};

#endif //FEM_RIGIDBODY_H
