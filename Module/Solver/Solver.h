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

enum class SolverType {
	kOptSolver,
};

struct SolverParameter {
	double _step;
	OptimizerType _opt_type;
	OptimizerParameter _opt_para;
	TargetParameter _tar_para;
};

class Solver {
public:
	void Initialize(const SolverParameter& para);
	void SetMesh(const Mesh& reference);
	virtual Target* CreateCorrespondingTarget() = 0;
	void Step(double dt);
	Mesh& GetCurrentMesh();

protected:
	// Data
	Mesh _reference;

	// Parameters
	double _step;
	Optimizer* _optimizer;
	Target* _target;

	// Status
	Mesh _current;
	VectorXd _v;
};

#endif //FEM_SOLVER_H
