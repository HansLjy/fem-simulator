//
// Created by hansljy on 2022/4/9.
//

#ifndef FEM_INTEGRATOR_H
#define FEM_INTEGRATOR_H

#include "System/System.h"
#include "Solver/Solver.h"
#include "Contact/ContactGenerator.h"
#include "BodyEnergy/BodyEnergy.h"

enum class IntegratorType {
	kLCPIntegrator
};

class IntegratorParameter {
public:
	DERIVED_DECLARE_CLONE(IntegratorParameter);
};

/**
 * Assemble, solve and then update the system status
 */
class Integrator {
public:
	virtual void Initialize(const IntegratorParameter& para) {};
	virtual void Step(System &system, const ContactGenerator &contact,
					  const BodyEnergy &body_energy, double h,
					  const Solver &solver) = 0;

	virtual ~Integrator() = default;
};

#endif //FEM_INTEGRATOR_H
