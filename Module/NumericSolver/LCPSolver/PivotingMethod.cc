//
// Created by hansljy on 2022/4/11.
//

#include "PivotingMethod.h"
#include <vector>
#include <spdlog/spdlog.h>
#include <limits>
#include <set>
#include <iostream>

DEFINE_CLONE(LCPSolverParameter, PivotingMethodParameter)

VectorXd
PivotingMethod::Solve(const MatrixXd &A, const VectorXd &b, const VectorXd &x0,
					  int block_size) const {
//	std::cerr << "A:\n" << A << std::endl << "b: " << b.transpose() << std::endl;

	VectorXd x;
	x.resizeLike(b);
	x.setZero();
	const int size = b.size();
	const double dbl_max = std::numeric_limits<double>::max();

	std::vector<int> A_set, F_set;
	std::set<int> unused_set;
	std::set<int> unfeasible_set;	// unfeasible set in selecting the proper y to pivot

	for (int i = 0; i < size; i++) {
		unused_set.insert(i);
	}

	int step = 0;
	int ready_count = 0;	// how many indices are currently in A union F
	while (step++ < _max_step) {
		VectorXd y = A * x + b;

		int F_old_size = F_set.size();
		for (auto &itr : unused_set) {
			if (y(itr) > 0) {
//				spdlog::info("Moving {} into F set", itr);
				F_set.push_back(itr);
			}
		}
		for (int i = F_old_size, F_size = F_set.size(); i < F_size; i++) {
			unused_set.erase(F_set[i]);
		}

//		spdlog::info("Round {} begins, starting info: ", step);
//		std::cerr << "x: " << x.transpose() << std::endl
//				  << "y: " << y.transpose() << std::endl;
//		std::cerr << "A_set: " << std::endl;
//		for (auto it : A_set) std::cerr << it << " ";
//		std::cerr << std::endl;
//		std::cerr << "F_set: " << std::endl;
//		for (auto it : F_set) std::cerr << it << " ";
//		std::cerr << std::endl;

		int j = -1;				// the index of y to make positive
		double min_y = 0;		// minimum of y
		for (auto &itr : unused_set) {
			if (y(itr) < min_y && unfeasible_set.find(itr) == unfeasible_set.end()) {
				j = itr;
				min_y = y(itr);
			}
		}
		if (j == -1) {
			// all positive or all negative y are unfeasible to make positive
			break;
		}

		VectorXd delta_x_A(A_set.size());
		if (!A_set.empty()) {
			delta_x_A = -A(A_set, A_set).colPivHouseholderQr().solve(
					A(A_set, j));
		}

		VectorXd delta_y_F = A(F_set, j) + A(F_set, A_set) * delta_x_A;
		double delta_y_j = A(j, j) + A(j, A_set) * delta_x_A;

		int bound_A_index = -1, bound_F_index = -1;
		double bound_A = dbl_max, bound_F = dbl_max, bound_j = dbl_max;
		for (int i = 0, A_size = A_set.size(); i < A_size; i++) {
			if (delta_x_A(i) < 0) {
				if (bound_A_index == -1 || - x(A_set[i]) / delta_x_A(i) < bound_A) {
					bound_A_index = i;
					bound_A = - x(A_set[i]) / delta_x_A(i);
				}
			}
		}
		for (int i = 0, F_size = F_set.size(); i < F_size; i++) {
			if (delta_y_F(i) < 0) {
				if (bound_F_index == -1 || - y(F_set[i]) / delta_y_F(i) < bound_F) {
					bound_F_index = i;
					bound_F = - y(F_set[i]) / delta_y_F(i);
				}
			}
		}
		if (delta_y_j > 0) {
			bound_j = - y(j) / delta_y_j;
		}
		if (bound_A_index == -1 && bound_F_index == -1 && delta_y_j < 1e-10) {
			unfeasible_set.insert(j);
//			spdlog::info("Round {} finished, Add {} into unfeasible set", step, j);
			if (unfeasible_set.size() > size) {
				break;
			}
		} else {
			unfeasible_set.clear();
			double bound = std::min(bound_A, std::min(bound_F, bound_j));
//			spdlog::info("Round {} finishes", step);
//			spdlog::info("Bound for this round: {}", bound);
//			spdlog::info("Selected j for this round: {}, change in this round:", j);
//			std::cerr << "delta x_A: " << delta_x_A.transpose() << std::endl;
//			std::cerr << "delta y_F: " << delta_y_F.transpose() << std::endl;
//			std::cerr << "delta y_j: " << delta_y_j << std::endl;
			x(A_set) += bound * delta_x_A;
			y(F_set) += bound * delta_y_F;
			y(j) += bound * delta_y_j;
			x(j) += bound;
			if (bound == bound_A) {
				int alter_id = A_set[bound_A_index];
				A_set.erase(A_set.begin() + bound_A_index);
				F_set.push_back(alter_id);
//				std::cerr << "Moving " << alter_id << " from A to F" << std::endl;
			} else if (bound == bound_F) {
				int alter_id = F_set[bound_F_index];
				A_set.push_back(alter_id);
				F_set.erase(F_set.begin() + bound_F_index);
//				std::cerr << "Moving " << alter_id << " from F to A" << std::endl;
			} else {
//				std::cerr << "Adding " << j << " into A" << std::endl;
				A_set.push_back(j);
				unused_set.erase(j);
				ready_count++;
			}
		}
		if (ready_count == size) {
			break;
		}
	}

	if (step < _max_step + 1) {
		if (unfeasible_set.empty()) {
			spdlog::info("Pivoting Method, converges in {} steps", step);
		} else {
			spdlog::error("Pivoting Method, run into unfeasible situation");
		}
	} else {
		spdlog::warn("Pivoting Method, fail to converge");
	}
	return x;
}
