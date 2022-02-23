//
// Created by hansljy on 2022/2/23.
//

#include "ElasticEnergy.h"
#include "Util/Factory.h"

void ElasticEnergy::Initialize(const ElasticEnergyParameter &para) {
	_cons_model = ConsistencyModelFactory::GetInstance()->GetConsistencyModel(para._cons_type);
	_cons_model->Initialize(para._cons_para);
}