//
// Created by hansljy on 2022/2/23.
//

#include "ConstituteModel.h"
#include <spdlog/spdlog.h>

ConstituteModelParameter::ConstituteModelParameter() = default;

DEFINE_VIRTUAL_ACCESSIBLE_MEMBER(ConsistencyModelParameter, double, YoungsModule)
DEFINE_VIRTUAL_ACCESSIBLE_MEMBER(ConsistencyModelParameter, double, PoissonRatio)

void ConstituteModel::Initialize(const ConstituteModelParameter &para) {
	spdlog::info("ConstituteModel initialized");
}

ConstituteModel::~ConstituteModel() = default;

