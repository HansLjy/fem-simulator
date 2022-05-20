//
// Created by hansljy on 22-5-18.
//

#ifndef FEM_CCD_H
#define FEM_CCD_H

#include "Contact/Surface.h"
#include "Util/Pattern.h"

using namespace SurfaceElements;

class CCDParameter {
public:
	CCDParameter(double tolerance, int max_iter) : _tolerance(tolerance), _max_iter(max_iter) {}
	DECLARE_ACCESSIBLE_MEMBER(double , Tolerance, _tolerance)
	DECLARE_ACCESSIBLE_MEMBER(int, MaxIter, _max_iter)
	DERIVED_DECLARE_CLONE(CCDParameter)
};

class CCD {
public:
	void Initialize(const CCDParameter& para);

	bool
	VertexFaceCollision(const Vertex &vertex_start, const Vertex &vertex_end,
						const Face &face_start, const Face &face_end,
						Vector3d &point, Vector3d &normal);

	bool EdgeEdgeCollision(const Edge &edge1_start, const Edge &edge1_end,
						   const Edge &edge2_start, const Edge &edge2_end,
						   Vector3d &point, Vector3d &normal);

protected:
	double _tolerance;
	int _max_iter;
};

#endif //FEM_CCD_H
