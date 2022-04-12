//
// Created by hansljy on 2022/3/11.
//

#ifndef FEM_EXTERNALFORCE_H
#define FEM_EXTERNALFORCE_H

#include "Mesh/Mesh.h"
#include "Util/EigenAll.h"
#include "SoftBody/SoftBody.h"

class ExternalForce {
public:
	virtual double Energy(const SoftBody &body) const = 0;
	virtual VectorXd Gradient(const SoftBody &body) const = 0;
	virtual SparseMatrixXd Hessian(const SoftBody &body) const = 0;

	virtual ~ExternalForce() = default;

	BASE_DECLARE_CLONE(ExternalForce);
};

#endif //FEM_EXTERNALFORCE_H
