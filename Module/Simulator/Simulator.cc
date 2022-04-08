//
// Created by hansljy on 2022/2/22.
//

#include "Simulator.h"
#include "Util/Factory.h"
#include <spdlog/spdlog.h>

SimulatorParameter::SimulatorParameter(const string &output_dir,
									   double duration, double step,
									   SolverType sol_type,
									   const SolverParameter&sol_para,
									   const MeshParameter& mesh_para)
									   : _output_dir(output_dir),
									     _duration(duration), _step(step),
										 _sol_type(sol_type),
										 _sol_para(sol_para.Clone()),
										 _mesh_para(mesh_para)
									   {}

DEFINE_ACCESSIBLE_MEMBER(SimulatorParameter, string, OutputDir, _output_dir)
DEFINE_ACCESSIBLE_MEMBER(SimulatorParameter, double, Duration, _duration)
DEFINE_ACCESSIBLE_MEMBER(SimulatorParameter, double, Step, _step)
DEFINE_ACCESSIBLE_MEMBER(SimulatorParameter, SolverType, SolverType, _sol_type)
DEFINE_ACCESSIBLE_POINTER_MEMBER(SimulatorParameter, SolverParameter, SolverParameter, _sol_para)
DEFINE_ACCESSIBLE_MEMBER(SimulatorParameter, MeshParameter, MeshParameter, _mesh_para)

void Simulator::Initialize(const SimulatorParameter &para) {
	_output_dir = para.GetOutputDir();
	_duration = para.GetDuration();
	_step = para.GetStep();
	_solver = SolverFactory::GetInstance()->GetSolver(para.GetSolverType());
	_solver->Initialize(*para.GetSolverParameter());

	Mesh mesh;
	mesh.Initialize(para.GetMeshParameter());
	_title = mesh.GetTitle();
	_solver->SetMesh(mesh);
}

void Simulator::Simulate() {
	double current = 0;
	string prefix = _output_dir + "/" + _title;
	string suffix = ".vtk";
	int index = 0;
	while(current < _duration) {
		current += _step;
		const auto& mesh = _solver->GetCurrentMesh();
		mesh.Store(prefix + std::to_string(index++) + suffix);
		_solver->Step(_step);
		spdlog::info("Frame id: {}", index);
	}
}

Simulator::~Simulator() {
	delete _solver;
}

void Simulator::AddConstraint(const Constraint &cons) {
	_solver->AddConstraint(cons);
}

void Simulator::AddExternalForce(const ExternalForce &ext) {
	_solver->AddExternalForce(ext);
}