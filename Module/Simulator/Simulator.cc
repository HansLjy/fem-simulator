//
// Created by hansljy on 2022/2/22.
//

#include "Simulator.h"
#include "Util/Factory.h"

void Simulator::Initialize(const SimulatorParameter &para) {
	_output_dir = para._output_dir;
	_duration = para._duration;
	_step = para._step;
	_solver = SolverFactory::GetInstance()->GetSolver(para._sol_type);
	_reference.Initialize(para._mesh_para);
	_solver->Initialize(para._sol_para);
	_solver->SetMesh(_reference);
}

void Simulator::Simulate() {
	double current = 0;
	string prefix = _output_dir + "/" + _reference.GetTitle();
	string suffix = ".vtk";
	int index = 0;
	while(current < _duration) {
		current += _step;
		_solver->Step(_step);
		const auto& mesh = _solver->GetCurrentMesh();
		mesh.Store(prefix + std::to_string(index) + suffix);
	}
}