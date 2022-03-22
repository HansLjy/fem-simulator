//
// Created by hansljy on 2022/2/22.
//

#ifndef FEM_MASSMODEL_H
#define FEM_MASSMODEL_H

#include "Mesh/Mesh.h"
#include "Util/Pattern.h"
#include "Util/EigenAll.h"

enum class MassModelType {
	kVoronoi
};

class MassModelParameter {
public:
	MassModelParameter(double density);
	BASE_DECLARE_CLONE(MassModelParameter)

	DECLARE_ACCESSIBLE_MEMBER_ACCESSOR(double, Density, _density)
};

class MassModel {
public:
	virtual void Initialize(const MassModelParameter& para);
	virtual VectorXd GetMassDistribution(const Mesh& mesh) = 0;

	virtual ~MassModel();
	BASE_DECLARE_CLONE(MassModel)

protected:
	double _density;
};

#endif //FEM_MASSMODEL_H
