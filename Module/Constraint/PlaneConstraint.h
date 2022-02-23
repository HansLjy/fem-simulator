//
// Created by hansljy on 2022/2/22.
//

#ifndef FEM_PLANECONSTRAINTS_H
#define FEM_PLANECONSTRAINTS_H

#include "Constraint/Constraint.h"

using Eigen::Vector3d;

class PlaneConstraint : public Constraint {
public:
	void SetPlane(const Vector3d& norm, const Vector3d& point);
	double Value(const VectorXd &x) override;
	VectorXd Gradient(const VectorXd &x) override;
	MatrixXd Hessian(const VectorXd &x) override;

private:
	Vector3d _norm, _point;
};

#endif //FEM_PLANECONSTRAINTS_H
