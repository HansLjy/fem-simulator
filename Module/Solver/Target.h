//
// Created by hansljy on 2022/2/22.
//

#ifndef FEM_TARGET_H
#define FEM_TARGET_H

#include "Function/Function.h"
#include "Mesh/Mesh.h"
#include "Util/Pattern.h"
#include "BodyEnergy/BodyEnergy.h"
#include "BodyEnergy/ExternalForce.h"
#include "Mass/MassModel.h"

using Eigen::Matrix;

enum class TargetType {
	kBackward,
};

class TargetParameter {
public:
	TargetParameter(const BodyEnergyParameter &body_para,
					const MassModelType &mass_type,
					const MassModelParameter &mass_para);

	DERIVED_DECLARE_CLONE(TargetParameter)
	DECLARE_ACCESSIBLE_POINTER_MEMBER_ACCESSOR(BodyEnergyParameter, BodyEnergyParameter, _body_para)
	DECLARE_ACCESSIBLE_MEMBER_ACCESSOR(MassModelType, MassModelType, _mass_type)
	DECLARE_ACCESSIBLE_POINTER_MEMBER_ACCESSOR(MassModelParameter, MassModelParameter, _mass_para)
};

class Target : public Function {
public:
	double Value(const VectorXd &x) const override = 0;
	VectorXd Gradient(const VectorXd &x) const override = 0;
	SparseMatrixXd Hessian(const VectorXd &x) const override = 0;

	virtual void Initialize(const TargetParameter& para);
	void AddExternalForce(const ExternalForce& ext);
	void SetMesh(const Mesh& mesh);

	void SetX(const VectorXd& x);	// set xn
	void SetV(const VectorXd& v);	// set vn
	void SetDt(double dt);

	Target() = default;

	~Target() override;
	Target(const Target& target);

protected:
	void PreCompute();
	void ComputeInverse();
	void ComputeMass();
	void ComputeVolumn();
	void ComputePd();

	// Data
	Mesh _reference;

	// Parameter
	BodyEnergy* _body_energy = nullptr;
	MassModel* _mass_model = nullptr;

	VectorXd _mass;
	VectorXd _mass_sparse;
	VectorX<Matrix3d> _inv;
	VectorX<Matrix12x9d> _pFpX;
	VectorXd _volumn;

	std::vector<const ExternalForce*> _ext_force;

	// Outer Status
	VectorXd _x, _v;
	double _dt = 0;
};

#endif //FEM_TARGET_H
