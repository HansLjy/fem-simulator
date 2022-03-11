//
// Created by hansljy on 2022/2/25.
//

#include "MassModel.h"
#include <spdlog/spdlog.h>

MassModelParameter::MassModelParameter() = default;
void MassModel::Initialize(const MassModelParameter &para) {
	spdlog::info("MassModel initialized");
}
MassModel::~MassModel() = default;