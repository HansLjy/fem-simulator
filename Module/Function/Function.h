//
// Created by hansljy on 2022/2/22.
//

#ifndef FEM_FUNCTION_H
#define FEM_FUNCTION_H

#include "Eigen/Dense"
#include <functional>

using Eigen::VectorXd;
using Eigen::MatrixXd;
using std::function;

class Function {
public:
	virtual double Value(const VectorXd& x) const = 0;
	virtual VectorXd Gradient(const VectorXd& x) const = 0;
	virtual MatrixXd Hessian(const VectorXd& x) const = 0;
};


#endif //FEM_FUNCTION_H
