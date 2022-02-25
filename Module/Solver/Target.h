//
// Created by hansljy on 2022/2/22.
//

#ifndef FEM_TARGET_H
#define FEM_TARGET_H

#include "Function/Function.h"
#include "Mesh/Mesh.h"
#include "Util/Pattern.h"
#include "BodyEnergy/BodyEnergy.h"
#include "Mass/MassModel.h"

enum class TargetType {
	kBackward,
};

class TargetParameter {
public:
	TargetParameter(const BodyEnergyParameter& body_para);

	DERIVED_DECLARE_CLONE(TargetParameter)
	DECLARE_ACCESSIBLE_POINTER_MEMBER_ACCESSOR(BodyEnergyParameter, BodyEnergyParameter, _body_para)
	DECLARE_ACCESSIBLE_MEMBER_ACCESSOR(MassModelType, MassModelType, _mass_type)
	DECLARE_ACCESSIBLE_POINTER_MEMBER_ACCESSOR(MassModelParameter, MassModelParameter, _mass_para)
};

class Target : public Function {
public:
	double Value(const VectorXd &x) const override = 0;
	VectorXd Gradient(const VectorXd &x) const override = 0;
	MatrixXd Hessian(const VectorXd &x) const override = 0;

	void Initialize(const TargetParameter& para);
	void SetMesh(const Mesh& mesh);

	void SetX(const VectorXd& x);	// set xn
	void SetV(const VectorXd& v);	// set vn
	void SetDt(double dt);

	Target() = default;

	~Target();
	Target(const Target& target);

protected:
	// Data
	Mesh _reference;

	// Parameter
	BodyEnergy* _body_energy;
	MassModel* _mass_model;

	// Outer Status
	VectorXd _x, _v;
	double _dt;
};

#endif //FEM_TARGET_H
