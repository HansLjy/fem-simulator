//
// Created by hansljy on 2022/2/23.
//

#include "ConsitencyModel.h"

ConsistencyModelParameter::ConsistencyModelParameter() = default;

DEFINE_VIRTUAL_ACCESSIBLE_MEMBER_ACCESSOR(ConsistencyModelParameter, double, YoungsModule)
DEFINE_VIRTUAL_ACCESSIBLE_MEMBER_ACCESSOR(ConsistencyModelParameter, double, PoissonRatio)

void ConsistencyModel::Initialize(const ConsistencyModelParameter &para) {
}

ConsistencyModel::~ConsistencyModel() = default;

