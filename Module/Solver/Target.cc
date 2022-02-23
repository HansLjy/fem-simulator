//
// Created by hansljy on 2022/2/23.
//

#include "Target.h"
#include "Util/Factory.h"

void Target::Initialize(const TargetParameter &para) {
	_elas_model = ElasticEnergyFactory::GetInstance()->GetElasticEnergy(para._elas_type);
	_elas_model->Initialize(para._elas_para);
	_diss_model = DissipationEnergyFactory::GetInstance()->GetDissipationEnergy(para._diss_type);
	_diss_model->Initialize(para._diss_para);
	_reference.ComputeMass();
}