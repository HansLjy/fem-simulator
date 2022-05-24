//
// Created by hansljy on 2022/3/11.
//

#ifndef FEM_EXTERNALFORCE_H
#define FEM_EXTERNALFORCE_H

#include "Util/EigenAll.h"

class Object;
class ExternalForce {
public:
	virtual double Energy(const Object &obj) const = 0;
	virtual VectorXd Gradient(const Object &obj) const = 0;
	virtual SparseMatrixXd Hessian(const Object &obj) const = 0;

	virtual ~ExternalForce() = default;

	BASE_DECLARE_CLONE(ExternalForce);

protected:
};

#endif //FEM_EXTERNALFORCE_H
