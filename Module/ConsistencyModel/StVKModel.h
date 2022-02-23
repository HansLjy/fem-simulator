//
// Created by hansljy on 2022/2/22.
//

#ifndef FEM_STVKMODEL_H
#define FEM_STVKMODEL_H

#include "ConsitencyModel.h"

class StVKModel : public ConsistencyModel {
public:
	double EnergyDensity(const Matrix3d &F) override;
	Matrix3d Piola(const Matrix3d &F) override;
	Matrix3d PiolaDifferential(const Matrix3d &F, const Matrix3d &dF) override;
};

#endif //FEM_STVKMODEL_H
