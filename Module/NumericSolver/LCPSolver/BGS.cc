//
// Created by hansljy on 2022/4/14.
//

#include "BGS.h"
#include "Util/Factory.h"
#include "PivotingMethod.h"
#include <spdlog/spdlog.h>

DEFINE_CLONE(LCPSolverParameter, BGSParameter)

VectorXd BGS::Solve(const MatrixXd &A, const VectorXd &b, const VectorXd &x0,
					int block_size) const {
	const int size = b.size();
	const int num_block = size / block_size;

	const auto& small_scale_solver = LCPSolverFactory::GetInstance()->GetLCPSolver(LCPSolverType::kPivot);
	small_scale_solver->Initialize(PivotingMethodParameter(_max_step, _max_error));
	VectorXd x = x0;

	int step = 0;
	while (step++ < _max_step) {
		for (int i = 0; i < num_block; i++) {
			VectorXd bi = b.block(i * block_size, 0, block_size, 1);
			for (int j = 0; j < num_block; j++) {
				if (j != i) {
					bi += A.block(i * block_size, j * block_size, block_size, block_size)
						  * x.block(j * block_size, 0, block_size, 1);
				}
			}
			x.block(i * block_size, 0, block_size, 1) = small_scale_solver->Solve(A.block(i * block_size, i * block_size, block_size, block_size), bi);
		}
		VectorXd y = A * x + b;
		bool y_positive = true;
		for (int i = 0; i < size; i++) {
			if (y(i) < 0) {
				y_positive = false;
				break;
			}
		}
		if (y_positive && x.dot(y) < _max_error) {
			break;
		}
	}

	if (step < _max_step + 1) {
		spdlog::info("BGS, converges in {} steps", step);
	} else {
		spdlog::warn("BGS, fail to converge");
	}

	delete small_scale_solver;

	return x;
}