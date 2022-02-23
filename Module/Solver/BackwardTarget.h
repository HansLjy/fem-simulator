//
// Created by hansljy on 2022/2/22.
//

#ifndef FEM_BACKWARDTARGET_H
#define FEM_BACKWARDTARGET_H

#include "Target.h"

class BackwardTarget : public Target {
public:
	double Value(const VectorXd &x) override;
	VectorXd Gradient(const VectorXd &x) override;
	MatrixXd Hessian(const VectorXd &x) override;
};

#endif //FEM_BACKWARDTARGET_H
