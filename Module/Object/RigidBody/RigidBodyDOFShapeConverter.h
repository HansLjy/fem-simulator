//
// Created by hansljy on 22-6-30.
//

#ifndef FEM_RIGIDBODYDOFSHAPECONVERTER_H
#define FEM_RIGIDBODYDOFSHAPECONVERTER_H

#include "Object/DOFShapeConverter.h"

class RigidBodyDOFShapeConverter : public DOFShapeConverter {
public:
	Matrix<int, Dynamic, 3> GetSurfaceTopo(const Object &obj) const override;

	MatrixXd GetSurfacePosition(const Object &obj) const override;

	SparseMatrixXd
	GetJ(const Object &obj, int idx, const Vector3d &point) const override = 0;

	void Store(const Object &obj, const std::string &filename,
			   const OutputFormatType &format) const override;
};

#endif //FEM_RIGIDBODYDOFSHAPECONVERTER_H
