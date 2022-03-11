//
// Created by hansljy on 2022/2/22.
//

#ifndef FEM_SOLVER_H
#define FEM_SOLVER_H

#include "Mesh/Mesh.h"
#include "Function/Function.h"
#include "Mass/MassModel.h"
#include "ElementEnergy/ElasticEnergy.h"
#include "ElementEnergy/DissipationEnergy.h"
#include "Optimizer/Optimizer.h"
#include "Target.h"
#include "Util/Pattern.h"
#include "BodyEnergy/ExternalForce.h"

enum class SolverType {
	kBackward,
};

class SolverParameter {
public:
	SolverParameter(OptimizerType opt_type, const OptimizerParameter& opt_para, const TargetParameter& tar_para);

	DERIVED_DECLARE_CLONE(SolverParameter)
	DECLARE_ACCESSIBLE_MEMBER_ACCESSOR(OptimizerType, OptimizerType, _opt_type)
	DECLARE_ACCESSIBLE_POINTER_MEMBER_ACCESSOR(OptimizerParameter, OptimizerParameter, _opt_para)
	DECLARE_ACCESSIBLE_POINTER_MEMBER_ACCESSOR(TargetParameter, TargetParameter, _tar_para)
};

class Solver {
public:
	Solver() = default;
	void Initialize(const SolverParameter& para);
	void AddConstraint(const Constraint& cons);
	void AddExternalForce(const ExternalForce& ext);
	void SetMesh(const Mesh& reference);
	virtual Target* CreateCorrespondingTarget() = 0;
	void Step(double dt);
	Mesh& GetCurrentMesh();

	virtual ~Solver();
	Solver(const Solver& solver);
	BASE_DECLARE_CLONE(Solver)

protected:
	// Data
	Mesh _reference;

	// Parameters
	Optimizer* _optimizer;
	Target* _target;

	// Status
	Mesh _current;
	VectorXd _v;
};

#endif //FEM_SOLVER_H
