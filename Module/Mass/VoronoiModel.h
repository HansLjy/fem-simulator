//
// Created by hansljy on 2022/2/22.
//

#ifndef FEM_VORONOIMODEL_H
#define FEM_VORONOIMODEL_H

#include "MassModel.h"

class VoronoiModelParameter : public MassModelParameter {
public:
	VoronoiModelParameter(double density);
	DERIVED_DECLARE_CLONE(MassModelParameter)
};

class VoronoiModel : public MassModel {
public:
	VectorXd GetMassDistribution(const Mesh &mesh) const override;
	~VoronoiModel();

	DERIVED_DECLARE_CLONE(MassModel)
};

#endif //FEM_VORONOIMODEL_H
