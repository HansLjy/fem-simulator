//
// Created by hansljy on 2022/2/22.
//

#ifndef FEM_SIMULATOR_H
#define FEM_SIMULATOR_H

#include <vector>
#include <string>
#include "Mesh/Mesh.h"
#include "Constraint/Constraint.h"
#include "ElementEnergy/DissipationEnergy.h"
#include "ElementEnergy/ElasticEnergy.h"
#include "Mass/MassModel.h"
#include "ConsistencyModel/ConsitencyModel.h"
#include "Optimizer/Optimizer.h"
#include "Solver/Solver.h"

using std::string;

struct SimulatorParameter {
	string _output_dir;
	double _duration;
	double _step;
	SolverType _sol_type;
	SolverParameter _sol_para;
	MeshParameter _mesh_para;
};



class Simulator {
public:
	void Initialize(const SimulatorParameter& para);
	void AddConstraint(const Constraint& cons);
	void Simulate();

private:
	string _output_dir;
	double _duration;
	double _step;
	Solver* _solver;
	Mesh _reference;

};

#endif //FEM_SIMULATOR_H
