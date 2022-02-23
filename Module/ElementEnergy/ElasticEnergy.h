//
// Created by hansljy on 2022/2/22.
//

#ifndef FEM_ELEMENTENERGY_H
#define FEM_ELEMENTENERGY_H

#include "Eigen/Dense"
#include "ConsistencyModel/ConsitencyModel.h"

using Eigen::Vector3d;
using Eigen::Matrix3d;

typedef Eigen::Vector<double, 12> Vector12d;
typedef Eigen::Matrix<double, 12, 12> Matrix12d;

enum class ElasticEnergyType {
	kSimple,
};

struct ElasticEnergyParameter {
	ConsistencyModelType _cons_type;
	ConsistencyModelParameter _cons_para;
};

class ElasticEnergy {
public:
	void Initialize(const ElasticEnergyParameter& para);

	// Here B is the inverse of [X1 - X4, X2 - X4, X3 - X4]
	virtual double Energy(const Matrix3d& B, const Matrix3d& Ds) const = 0;
	virtual Vector12d Gradient(const Matrix3d& B, const Matrix3d& Ds) const = 0;
	virtual Matrix12d Hessian(const Matrix3d& B, const Matrix3d& Ds) const = 0;

	virtual ~ElasticEnergy();

protected:
	ConsistencyModel* _cons_model;
};

#endif //FEM_ELEMENTENERGY_H
