//
// Created by hansljy on 2022/2/23.
//

#include "Target.h"
#include "Util/Factory.h"

void Target::Initialize(const TargetParameter &para) {
	_body_energy = new BodyEnergy;
	_body_energy->Initialize(para._body_para);
	_reference.ComputeMass();
}