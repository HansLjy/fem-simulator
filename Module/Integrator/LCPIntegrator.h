//
// Created by hansljy on 2022/4/9.
//

#ifndef FEM_LCPINTEGRATOR_H
#define FEM_LCPINTEGRATOR_H

#include "Integrator.h"

class LCPIntegratorParameter : public IntegratorParameter {
public:
	LCPIntegratorParameter(const LCPSolverType& lcp_type, const LCPSolverParameter& lcp_para)
	: _lcp_type(lcp_type), _lcp_para(lcp_para.Clone()) {}

	DERIVED_DECLARE_CLONE(IntegratorParameter)

	LCPIntegratorParameter(const LCPIntegratorParameter& rhs) {
		_lcp_type = rhs._lcp_type;
		_lcp_para = rhs.GetLCPSolverParameter()->Clone();
	}

	~LCPIntegratorParameter() override {
		delete _lcp_para;
	}

	DECLARE_OVERWRITE_ACCESSIBLE_MEMBER(LCPSolverType, LCPSolverType, _lcp_type)
	DECLARE_OVERWRITE_ACCESSIBLE_POINTER_MEMBER(LCPSolverParameter, LCPSolverParameter, _lcp_para)
};

class LCPIntegrator : public Integrator {
public:
	void Initialize(const IntegratorParameter &para) override;
	void Step(System &system, const ContactGenerator &contact, const BodyEnergy &body_energy, double h) override = 0;
	virtual ~LCPIntegrator() {
		delete _solver;
	}

protected:
	LCPSolver* _solver;
};

#endif //FEM_LCPINTEGRATOR_H
