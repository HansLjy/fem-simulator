//
// Created by hansljy on 2022/4/11.
//

#include "PivotingMethod.h"
#include <vector>
#include <spdlog/spdlog.h>
#include <limits>

DEFINE_CLONE(LCPSolverParameter, PivotingMethodParameter)

VectorXd
PivotingMethod::Solve(const MatrixXd &A, const VectorXd &b, const VectorXd &x0,
					  int block_size) const {
	VectorXd x;
	x.resizeLike(b);
	x.setZero();
	const int size = b.size();
	const double dbl_max = std::numeric_limits<double>::max();

	std::vector<int> A_set, F_set;
	int step = 0, ready_count = 0;
	while (step++ < _max_step) {
		VectorXd y = A * x + b;
		int id = -1;		// the index of y to make positive
		double min_y = 0;	// minimum of y
		for (int i = 0; i < size; i++) {
			if (y(i) < min_y) {
				id = i;
				min_y = y(i);
				break;
			}
		}
		if (id == -1) { // all positive
			break;
		}
		VectorXd delta_x_A = -A(A_set, A_set).colPivHouseholderQr().solve(A(A_set, id));
		VectorXd delta_y_F = A(F_set, id) + A(F_set, A_set) * delta_x_A;
		double delta_y_j = A(id, id) + A(id, A_set) * delta_x_A;

		int bound_A_index = -1, bound_F_index = -1;
		double bound_A = dbl_max, bound_F = dbl_max, bound_j = dbl_max;
		for (int i = 0; i < A_set.size(); i++) {
			if (delta_x_A(i) < 0) {
				if (bound_A_index == -1 || - x(A_set[i]) / delta_x_A(i) < bound_A) {
					bound_A_index = i;
					bound_A = - x(A_set[i]) / delta_x_A(i);
				}
			}
		}
		for (int i = 0; i < F_set.size(); i++) {
			if (delta_y_F(i) < 0) {
				if (bound_F_index == -1 || - y(F_set[i]) / delta_y_F(i) < bound_F) {
					bound_F_index = i;
					bound_F = - y(F_set[i]) / delta_y_F(i);
				}
			}
		}
		if (delta_y_j > 0) {
			bound_j = - y(id) / delta_y_j;
		}
		if (bound_A_index == -1 && bound_F_index == -1 && delta_y_j < 0) {
			spdlog::error("Pivoting Method, run into unfeasible situation");
			break;
		} else {
			double bound = std::min(bound_A, std::min(bound_F, bound_j));
			x(A_set) += bound * delta_x_A;
			y(F_set) += bound * delta_y_F;
			y(id) += bound * delta_y_j;
			if (bound_A < bound_F && bound_A < bound_j) {
				int alter_id = A_set[bound_A_index];
				A_set.erase(A_set.begin() + bound_A_index);
				F_set.push_back(alter_id);
			} else if (bound_F < bound_A && bound_F < bound_j) {
				int alter_id = F_set[bound_F_index];
				A_set.push_back(alter_id);
				F_set.erase(F_set.begin() + bound_F_index);
			} else {
				A_set.push_back(id);
				ready_count++;
			}
		}
		if (ready_count == size) {
			break;
		}
	}

	if (step < _max_step + 1) {
		spdlog::info("Pivoting Method, converges in {} steps", step);
	} else {
		spdlog::warn("Pivoting Method, fail to converge");
	}
	return x;
}
