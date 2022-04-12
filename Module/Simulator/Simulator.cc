//
// Created by hansljy on 2022/2/22.
//

#include "Simulator.h"
#include "Util/Factory.h"
#include <spdlog/spdlog.h>

DEFINE_ACCESSIBLE_MEMBER(SimulatorParameter, double, Duration, _duration)
DEFINE_ACCESSIBLE_MEMBER(SimulatorParameter, double, Step, _step)
DEFINE_ACCESSIBLE_MEMBER(SimulatorParameter, string, OutputDir, _output_dir)
DEFINE_ACCESSIBLE_MEMBER(SimulatorParameter, SystemParameter, SystemPara, _system_para)
DEFINE_ACCESSIBLE_MEMBER(SimulatorParameter, IntegratorType, IntegratorType, _integrator_type)
DEFINE_ACCESSIBLE_MEMBER(SimulatorParameter, IntegratorParameter, IntegratorPara, _integrator_para)
DEFINE_ACCESSIBLE_MEMBER(SimulatorParameter, ContactGeneratorType, ContactGenType, _contact_gen_type)
DEFINE_ACCESSIBLE_MEMBER(SimulatorParameter, ContactGeneratorParameter, ContactGenPara, _contact_gen_para)
DEFINE_ACCESSIBLE_MEMBER(SimulatorParameter, BodyEnergyParameter, BodyEngPara, _body_eng_para)
DEFINE_ACCESSIBLE_MEMBER(SimulatorParameter, ProblemType, ProbType, _prob_type)
DEFINE_ACCESSIBLE_MEMBER(SimulatorParameter, SolverType, SolverType, _sol_type)

void Simulator::Initialize(const SimulatorParameter &para) {
	_duration = para.GetDuration();
	_step = para.GetStep();
	_output_dir = para.GetOutputDir();

	_system.Initialize(para.GetSystemPara());

	_integrator = IntegratorFactory::GetInstance()->GetIntegrator(para.GetIntegratorType());
	_integrator->Initialize(para.GetIntegratorPara());

	_contact = ContactGeneratorFactory::GetInstance()->GetContactGenerator(para.GetContactGenType());
	_contact->Initialize(para.GetContactGenPara());

	_body_energy->Initialize(para.GetBodyEngPara());

	switch (para.GetProbType()) {
		case ProblemType::kLCP:
			_solver.LCP = LCPSolverFactory::GetInstance()->GetLCPSolver(para.GetSolverType().LCP);
		default:
			exit(1);
	}
}

void Simulator::Simulate() {
	double current = 0;
	int index = 0;
	while(current < _duration) {
		current += _step;
		_system.Store(_output_dir, index);
		_integrator->Step(_system, *_contact, *_body_energy, _step, _solver);
		spdlog::info("Frame id: {}", index++);
	}
}

Simulator::~Simulator() {
	delete _integrator;
	delete _contact;
	delete _body_energy;
}

void Simulator::AddRigidBody(const RigidBody &body) {
	_system.AddRigidBody(body);
}

void Simulator::AddExternalForce(const ExternalForce &ext) {
	_system.AddExternalForce(ext);
}