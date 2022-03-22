//
// Created by hansljy on 2022/3/21.
//

#ifndef FEM_NUMERICDIFFERENTIAL_H
#define FEM_NUMERICDIFFERENTIAL_H

#include "Util/EigenAll.h"
#include <functional>
using std::function;

typedef std::function<double(VectorXd)> VecFunction;

class NumericDifferential {
public:
	NumericDifferential(double delta);
	void SetFuntion(const VecFunction& func);

	VectorXd NumGradient(VectorXd x);
	MatrixXd NumHessian(VectorXd x);

private:
	double _delta;
	function<double(VectorXd)> _func;
};
#endif //FEM_NUMERICDIFFERENTIAL_H
