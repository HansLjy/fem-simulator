//
// Created by hansljy on 2022/2/22.
//

#ifndef FEM_TARGET_H
#define FEM_TARGET_H

#include "Function/Function.h"
#include "Mesh/Mesh.h"
#include "Util/Pattern.h"
#include "BodyEnergy/BodyEnergy.h"

enum class TargetType {
	kBackward,
};

struct TargetParameter {
	BodyEnergyParameter _body_para;
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

	BASE_DECLARE_CLONE(Target)

	Target() = default;

	virtual ~Target();
	Target(const Target& target);

protected:
	// Data
	Mesh _reference;

	// Parameter
	BodyEnergy* _body_energy;

	// Outer Status
	VectorXd _x, _v;
	double _dt;
};

#endif //FEM_TARGET_H
