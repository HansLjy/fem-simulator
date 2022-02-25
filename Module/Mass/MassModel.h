//
// Created by hansljy on 2022/2/22.
//

#ifndef FEM_MASSMODEL_H
#define FEM_MASSMODEL_H

#include "Eigen/Dense"
#include "Mesh/Mesh.h"
#include "Util/Pattern.h"

using Eigen::VectorXd;

enum class MassModelType {
	kVoronoi
};

class MassModelParameter {
public:
	MassModelParameter();

	DERIVED_DECLARE_CLONE(MassModelParameter)
};

class MassModel {
public:
	virtual void Initialize(const MassModelParameter& para);
	virtual VectorXd GetMassDistribution(const Mesh& mesh) = 0;

	virtual ~MassModel();
	BASE_DECLARE_CLONE(MassModel)
};

#endif //FEM_MASSMODEL_H
