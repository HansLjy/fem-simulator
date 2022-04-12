//
// Created by hansljy on 2022/4/11.
//

#ifndef FEM_PIVOT_H
#define FEM_PIVOT_H

#include "LCPSolver.h"

class PivotingMethod : public LCPSolver {
public:
	VectorXd Solve(const SparseMatrixXd &A, const VectorXd &b, const VectorXd &x0 = VectorXd()) const override;
};

#endif //FEM_PIVOT_H
