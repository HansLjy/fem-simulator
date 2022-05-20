//
// Created by hansljy on 22-5-20.
//

#include "DCD.h"

DEFINE_CLONE(DCDParameter, DCDParameter)
DEFINE_ACCESSIBLE_MEMBER(DCDParameter, int, MaxIter, _max_iter)
DEFINE_ACCESSIBLE_MEMBER(DCDParameter, double, Tolerance, _tolerance)

void DCD::Initialize(const DCDParameter &para) {
	_max_iter = para.GetMaxIter();
	_tolerance = para.GetTolerance();
}