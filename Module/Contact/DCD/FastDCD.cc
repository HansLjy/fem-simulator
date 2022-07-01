//
// Created by hansljy on 22-5-20.
//

#include "FastDCD.h"
#include "tritritest.h"

void FastDCD::Initialize(const DCDParameter &para) {
	DCD::Initialize(para);
}

bool FastDCD::GetIntersected(Vector3d A1, Vector3d B1, Vector3d C1, Vector3d A2,
							 Vector3d B2, Vector3d C2, Vector3d &point,
							 Vector3d &normal) {
	int coplanar = 0;
	real end_point1[3], end_point2[3];
	real tri1[3][3], tri2[3][3];
	for (int i = 0; i < 3; i++) {
		tri1[0][i] = A1(i);
	}
	for (int i = 0; i < 3; i++) {
		tri1[1][i] = B1(i);
	}
	for (int i = 0; i < 3; i++) {
		tri1[2][i] = C1(i);
	}
	for (int i = 0; i < 3; i++) {
		tri2[0][i] = A2(i);
	}
	for (int i = 0; i < 3; i++) {
		tri2[1][i] = B2(i);
	}
	for (int i = 0; i < 3; i++) {
		tri2[2][i] = C2(i);
	}

	int intersected = tri_tri_intersection_test_3d(tri1[0], tri1[1], tri1[2],
												   tri2[0], tri2[1], tri2[2],
												   &coplanar, end_point1,
												   end_point2);
	if (intersected) {
		for (int i = 0; i < 3; i++) {
			point[i] = end_point1[i];
		}
		normal = (B1 - A1).cross(C1 - A1).normalized();
		return true;
	} else {
		return false;
	}
}