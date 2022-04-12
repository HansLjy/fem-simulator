//
// Created by hansljy on 2022/4/9.
//

#ifndef FEM_LCPINTEGRATOR_H
#define FEM_LCPINTEGRATOR_H

#include "Integrator.h"

class LCPIntegrator : public Integrator {
public:
	void Step(System &system, const ContactGenerator &contact,
			  const BodyEnergy &body_energy, double h,
			  const Solver &solver) override;

};

#endif //FEM_LCPINTEGRATOR_H
