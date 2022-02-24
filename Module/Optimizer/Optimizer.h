//
// Created by hansljy on 2022/2/22.
//

#ifndef FEM_OPTIMIZER_H
#define FEM_OPTIMIZER_H

#include "Function/Function.h"
#include "Constraint/Constraint.h"
#include "Util/Pattern.h"

enum class OptimizerType {
	kInteriorPoint
};

struct OptimizerParameter {
	double _max_error;
	int _max_step;

};

class Optimizer {
public:
	void Initialize(const OptimizerParameter& para);
	void SetTarget(const Function& func);
	void AddConstraint(const Constraint& cons);
	void SetInitial(const VectorXd& x);
	virtual VectorXd Optimize(const VectorXd& x0) const = 0;

	virtual ~Optimizer();

	BASE_DECLARE_CLONE(Optimizer)

protected:
	// Parameter
	double _max_error;
	double _max_step;

	const Function* _target;
	std::vector<const Constraint*> _constraints;

};

#endif //FEM_OPTIMIZER_H
