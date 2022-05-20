//
// Created by hansljy on 22-5-6.
//

#ifndef FEM_POLYGONFRICTIONMODEL_H
#define FEM_POLYGONFRICTIONMODEL_H

#include "FrictionModel.h"

class PolygonFrictionModelParameter : public FrictionModelParameter {
public:
	PolygonFrictionModelParameter(int num_tangent) : _num_tangent(num_tangent) {}
	DERIVED_DECLARE_CLONE(FrictionModelParameter)
	DECLARE_OVERWRITE_ACCESSIBLE_MEMBER(int, NumTangent, _num_tangent)
};

class PolygonFrictionModel : public FrictionModel {
public:
	void Initialize(const FrictionModelParameter &para) override;
	void
	GetJ(const System &system, const vector<ContactPoint> &contacts, double h,
		 SparseMatrixXd &JnT, SparseMatrixXd &JtT, VectorXd &Mu) const override;

	int GetNumTangent() const override;

protected:
	int _num_tangent;
	vector<double> _sin, _cos;
};

#endif //FEM_POLYGONFRICTIONMODEL_H
