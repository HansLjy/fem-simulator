//
// Created by hansljy on 2022/2/23.
//

#include "ConsitencyModel.h"

void ConsistencyModel::Initialize(const ConsistencyModelParameter &para) {
	_lame_mu = para._youngs_module / (2 * (1 + para._poisson_ratio));
	_lame_lambda = para._youngs_module * para._poisson_ratio / ((1 + para._poisson_ratio) * (1 - 2 * para._poisson_ratio));
}

