//
// Created by hansljy on 2022/2/22.
//

#ifndef FEM_MASSMODEL_H
#define FEM_MASSMODEL_H

#include "Eigen/Dense"
#include "Mesh/Mesh.h"

enum class MassModelType {
	kVoronoi
};

class MassModel {
public:
	virtual VectorXd GetMassDistribution(const Mesh& mesh);
};

#endif //FEM_MASSMODEL_H
