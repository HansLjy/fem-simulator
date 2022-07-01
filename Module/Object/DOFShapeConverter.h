//
// Created by hansljy on 22-5-6.
//

#ifndef FEM_DOFSHAPECONVERTER_H
#define FEM_DOFSHAPECONVERTER_H

#include <vector>
#include "Util/EigenAll.h"
#include "Util/Pattern.h"

class Object;
enum class OutputFormatType {
	kVtk,
};

/**
 * Surface model deals mainly with the transformation
 * between DOF and actual shape of the objects.
 */
class DOFShapeConverter {
public:
	DOFShapeConverter() = default;

	// From DOF to shape
	virtual Matrix<int, Dynamic, 3> GetSurfaceTopo(const Object &obj) const = 0;
	virtual MatrixXd GetSurfacePosition(const Object &obj) const = 0;

	// From shape to DOF
	virtual SparseMatrixXd
	GetJ(const Object &obj, int idx, const Vector3d &point) const = 0;

	virtual void Store(const Object &obj, const std::string &filename,
					   const OutputFormatType &format) const = 0;

	virtual ~DOFShapeConverter() = default;

	BASE_DECLARE_CLONE(DOFShapeConverter)
};

#endif //FEM_DOFSHAPECONVERTER_H
