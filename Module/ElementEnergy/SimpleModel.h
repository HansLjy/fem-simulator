//
// Created by hansljy on 2022/2/22.
//

#ifndef FEM_SIMPLEMODEL_H
#define FEM_SIMPLEMODEL_H

#include "ElasticEnergy.h"

class SimpleModel : public ElasticEnergy {
public:
	double Energy(const Matrix3d &B, const Matrix3d &Ds) const override;
	Vector12d Gradient(const Matrix3d &B, const Matrix3d &Ds) const override;
	Matrix12d Hessian(const Matrix3d &B, const Matrix3d &Ds) const override;
};

#endif //FEM_SIMPLEMODEL_H
