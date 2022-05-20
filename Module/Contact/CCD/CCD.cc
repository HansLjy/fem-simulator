//
// Created by hansljy on 22-5-18.
//

#include "CCD.h"

DEFINE_ACCESSIBLE_MEMBER(CCDParameter, double , Tolerance, _tolerance)
DEFINE_ACCESSIBLE_MEMBER(CCDParameter, int, MaxIter, _max_iter)
DEFINE_CLONE(CCDParameter, CCDParameter)

void CCD::Initialize(const CCDParameter &para) {
	_tolerance = para.GetTolerance();
	_max_iter = para.GetMaxIter();
}

/**
 * For now, I use CCD-Wrappers. But I will implement
 * my own CCD algorithm one day.
 */

bool CCD::EdgeEdgeCollision(const Edge &edge1_start, const Edge &edge1_end,
							const Edge &edge2_start, const Edge &edge2_end,
							Vector3d &point, Vector3d &normal) {
	// TODO
}

bool
CCD::VertexFaceCollision(const Vertex &vertex_start, const Vertex &vertex_end,
						 const Face &face_start, const Face &face_end,
						 Vector3d &point, Vector3d &normal) {
	// TODO
}