//
// Created by hansljy on 2022/4/26.
//

#ifndef TEST_STAGGERLCPINTEGRATOR_H
#define TEST_STAGGERLCPINTEGRATOR_H

#include "LCPIntegrator.h"

class StaggerLCPIntegrator : public LCPIntegrator {
public:
	void Initialize(const IntegratorParameter &para) override;
	void Step(System &system, const ContactGenerator &contact, const BodyEnergy &body_energy, double h) override;

protected:
	int _max_step;
	double _max_error;
};

#endif //TEST_STAGGERLCPINTEGRATOR_H
