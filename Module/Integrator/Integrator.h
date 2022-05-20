//
// Created by hansljy on 2022/4/9.
//

#ifndef FEM_INTEGRATOR_H
#define FEM_INTEGRATOR_H

#include "System/System.h"
#include "NumericSolver/LCPSolver/LCPSolver.h"
#include "NumericSolver/Optimizer/Optimizer.h"
#include "Contact/ContactGenerator.h"
#include "Contact/FrictionModel.h"
#include "BodyEnergy/BodyEnergy.h"

enum class IntegratorType {
	kNonFrictionLCP,
	kStaggeringLCP
};

class IntegratorParameter {
public:
	BASE_DECLARE_CLONE(IntegratorParameter);

	DECLARE_VIRTUAL_ACCESSIBLE_MEMBER(LCPSolverType, LCPSolverType)
	DECLARE_VIRTUAL_ACCESSIBLE_POINTER_MEMBER(LCPSolverParameter, LCPSolverParameter)
	DECLARE_VIRTUAL_ACCESSIBLE_MEMBER(OptimizerType, OptimizerType)
	DECLARE_VIRTUAL_ACCESSIBLE_POINTER_MEMBER(OptimizerParameter, OptimizerParameter)

	virtual ~IntegratorParameter() = default;
};

/**
 * Assemble, solve and then update the system status
 */
class Integrator {
public:
	virtual void Initialize(const IntegratorParameter& para) = 0;
	virtual void Step(System &system, const ContactGenerator &contact_generator,
					  const FrictionModel &friction_model, double h) = 0;

	virtual ~Integrator() = default;
};

#endif //FEM_INTEGRATOR_H
