//
// Created by hansljy on 2022/2/22.
//

#ifndef FEM_SIMULATOR_H
#define FEM_SIMULATOR_H

#include <vector>
#include <string>
#include "System/System.h"
#include "Integrator/Integrator.h"
#include "Mass/MassModel.h"
#include "NumericSolver/LCPSolver/LCPSolver.h"

using std::string;

class SimulatorParameter {
public:
	SimulatorParameter(
			double duration,
			double step,
			const string& output_dir,
			const SystemParameter& system_para,
			IntegratorType integrator_type,
			const IntegratorParameter& integrator_para,
			ContactGeneratorType contact_type,
			const ContactGeneratorParameter& contact_para,
			const BodyEnergyParameter& body_energy_para,
			ProblemType prob_type,
			SolverType sol_type
	) : _duration(duration),
		_step(step),
		_output_dir(output_dir),
		_system_para(system_para.Clone()),
		_integrator_type(integrator_type),
		_integrator_para(integrator_para.Clone()),
		_contact_gen_type(contact_type),
		_contact_gen_para(contact_para.Clone()),
		_body_eng_para(body_energy_para.Clone()),
		_prob_type(prob_type),
		_sol_type(sol_type)
	{}

	DECLARE_ACCESSIBLE_MEMBER(double, Duration, _duration)
	DECLARE_ACCESSIBLE_MEMBER(double, Step, _step)
	DECLARE_ACCESSIBLE_MEMBER(string, OutputDir, _output_dir)
	DECLARE_ACCESSIBLE_POINTER_MEMBER(SystemParameter, SystemPara, _system_para)
	DECLARE_ACCESSIBLE_MEMBER(IntegratorType, IntegratorType, _integrator_type)
	DECLARE_ACCESSIBLE_POINTER_MEMBER(IntegratorParameter, IntegratorPara, _integrator_para)
	DECLARE_ACCESSIBLE_MEMBER(ContactGeneratorType, ContactGenType, _contact_gen_type)
	DECLARE_ACCESSIBLE_POINTER_MEMBER(ContactGeneratorParameter, ContactGenPara, _contact_gen_para)
	DECLARE_ACCESSIBLE_POINTER_MEMBER(BodyEnergyParameter, BodyEngPara, _body_eng_para)
	DECLARE_ACCESSIBLE_MEMBER(ProblemType, ProbType, _prob_type)
	DECLARE_ACCESSIBLE_MEMBER(SolverType, SolverType, _sol_type)

	~SimulatorParameter() {
		delete _system_para;
		delete _integrator_para;
		delete _contact_gen_para;
		delete _body_eng_para;
	}
};



class Simulator {
public:
	void Initialize(const SimulatorParameter& para);
	void AddRigidBody(const RigidBody& body);
	void AddExternalForce(const ExternalForce& ext);
	void Simulate();

	Simulator(const Simulator&) = delete;
	~Simulator();

private:
	double _duration;
	double _step;
	string _output_dir;

	System _system;

	Integrator* _integrator = nullptr;
	ContactGenerator* _contact = nullptr;
	BodyEnergy* _body_energy = nullptr;
	Solver _solver;	// Note this is also a base class, I just use a union instead
};

#endif //FEM_SIMULATOR_H
