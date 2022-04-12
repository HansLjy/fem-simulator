//
// Created by hansljy on 2022/2/25.
//

#include "DissipationEnergy.h"
#include <spdlog/spdlog.h>

DissipationEnergyModelParameter::DissipationEnergyModelParameter() = default;

void DissipationEnergyModel::Initialize(
		const DissipationEnergyModelParameter &para) {
	spdlog::info("DissipationEnergyModel initialized");
}

DEFINE_VIRTUAL_ACCESSIBLE_MEMBER(DissipationEnergyModelParameter, double, Alpha1)
DEFINE_VIRTUAL_ACCESSIBLE_MEMBER(DissipationEnergyModelParameter, double, Alpha2)