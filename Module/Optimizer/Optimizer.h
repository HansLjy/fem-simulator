//
// Created by hansljy on 2022/2/22.
//

#ifndef FEM_OPTIMIZER_H
#define FEM_OPTIMIZER_H

#include "Function/Function.h"
#include "Constraint/Constraint.h"

enum class OptimizerType {
	kInteriorPoint
};

struct OptimizerParameter {
	double _max_error;
	int _max_step;

};

class Optimizer {
public:
	void SetTarget(const Function& func);
	void AddConstraint(const Constraint& cons);
	void SetInitial(const VectorXd& x);
	void SetParameter(const OptimizerParameter& para);
	virtual VectorXd Optimize();

	virtual ~Optimizer();
};

#endif //FEM_OPTIMIZER_H
