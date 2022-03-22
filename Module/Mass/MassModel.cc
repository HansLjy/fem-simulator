//
// Created by hansljy on 2022/2/25.
//

#include "MassModel.h"
#include <spdlog/spdlog.h>

DEFINE_ACCESSIBLE_MEMBER(MassModelParameter, double, Density, _density)

MassModelParameter::MassModelParameter(double density) : _density(density) {}

void MassModel::Initialize(const MassModelParameter &para) {
	_density = para.GetDensity();
	spdlog::info("MassModel initialized");
}
MassModel::~MassModel() = default;