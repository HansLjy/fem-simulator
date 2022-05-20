//
// Created by hansljy on 2022/4/26.
//

#ifndef TEST_NONFRICLCPINTEGRATOR_H
#define TEST_NONFRICLCPINTEGRATOR_H

#include "LCPIntegrator.h"

class NonFricLCPIntegrator : public LCPIntegrator {
public:
	void Step(System &system, const ContactGenerator &contact_generator,
			  const FrictionModel &friction_model, double h) override;
};

#endif //TEST_NONFRICLCPINTEGRATOR_H
