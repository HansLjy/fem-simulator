//
// Created by hansljy on 22-5-20.
//

#include "FastDCD.h"
#include "tritritest.h"

void FastDCD::Initialize(const DCDParameter &para) {
	DCD::Initialize(para);
}

bool FastDCD::GetIntersected(const SurfaceElements::Face &face1,
							 const SurfaceElements::Face &face2,
							 Vector3d &point, Vector3d &normal) {
	int coplanar = 0;
	real end_point1[3], end_point2[3];
	real tri1[3][3], tri2[3][3];
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			tri1[i][j] = face1._vertex[i][j];
			tri2[i][j] = face2._vertex[i][j];
		}
	}
	int intersected = tri_tri_intersection_test_3d(tri1[0], tri1[1], tri1[2],
												   tri2[0], tri2[1], tri2[2],
												   &coplanar, end_point1,
												   end_point2);
	if (intersected) {
		for (int i = 0; i < 3; i++) {
			point[i] = end_point1[i];
		}
		normal = (face1._vertex[1] - face1._vertex[0]).cross(face1._vertex[2] - face1._vertex[0]).normalized();
		return true;
	} else {
		return false;
	}
}