//
// Created by hansljy on 2022/2/22.
//

#ifndef FEM_DISSIPATIONENERGY_H
#define FEM_DISSIPATIONENERGY_H

#include "ConsistencyModel/ConsitencyModel.h"

enum class DissipationEnergyType {
	kRayleigh
};

struct DissipationEnergyParameter {
	ConsistencyModelType _cons_type;
	ConsistencyModelParameter _cons_para;
};

class DissipationEnergy {
public:
	void Initialize(const DissipationEnergyParameter& para);

	double Energy();
	virtual ~DissipationEnergy();
};

#endif //FEM_DISSIPATIONENERGY_H
