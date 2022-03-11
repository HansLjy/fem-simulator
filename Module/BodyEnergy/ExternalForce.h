//
// Created by hansljy on 2022/3/11.
//

#ifndef FEM_EXTERNALFORCE_H
#define FEM_EXTERNALFORCE_H

#include "Mesh/Mesh.h"
#include "Eigen/Dense"

using Eigen::VectorXd, Eigen::MatrixXd;

class ExternalForce {
public:
	virtual double Energy (const Mesh& mesh, const VectorXd& mass, const VectorXd& X, const VectorXd& V) const = 0;
	virtual VectorXd Gradient (const Mesh& mesh, const VectorXd& mass, const VectorXd& X, const VectorXd& V) const = 0;
	virtual MatrixXd Hessian (const Mesh& mesh, const VectorXd& mass, const VectorXd& X, const VectorXd& V) const = 0;

	virtual ~ExternalForce() = default;

	BASE_DECLARE_CLONE(ExternalForce);
};

#endif //FEM_EXTERNALFORCE_H
