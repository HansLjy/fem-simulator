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
#include "BodyEnergy/ExternalForce.h"

using std::string;

class SimulatorParameter {
public:
	SimulatorParameter(
		const string& output_dir,
		double duration,
		double step,
		SolverType sol_type,
		const SolverParameter& sol_para,
		const MeshParameter& mesh_para
	);

	DECLARE_ACCESSIBLE_MEMBER_ACCESSOR(string, OutputDir, _output_dir)
	DECLARE_ACCESSIBLE_MEMBER_ACCESSOR(double, Duration, _duration)
	DECLARE_ACCESSIBLE_MEMBER_ACCESSOR(double, Step, _step)
	DECLARE_ACCESSIBLE_MEMBER_ACCESSOR(SolverType, SolverType, _sol_type)
	DECLARE_ACCESSIBLE_POINTER_MEMBER_ACCESSOR(SolverParameter, SolverParameter, _sol_para)
	DECLARE_ACCESSIBLE_MEMBER_ACCESSOR(MeshParameter, MeshParameter, _mesh_para)

};



class Simulator {
public:
	void Initialize(const SimulatorParameter& para);
	void AddConstraint(const Constraint& cons);
	void AddExternalForce(const ExternalForce& ext);
	void Simulate();
	~Simulator();

private:
	string _output_dir;
	double _duration;
	double _step;
	Solver* _solver;
	string _title;
};

#endif //FEM_SIMULATOR_H
