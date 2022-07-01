//
// Created by hansljy on 22-5-20.
//

#ifndef FEM_FASTDCD_H
#define FEM_FASTDCD_H

#include "DCD.h"

class FastDCD : public DCD {
	void Initialize(const DCDParameter &para) override;

	bool GetIntersected(Vector3d A1, Vector3d B1, Vector3d C1, Vector3d A2,
						Vector3d B2, Vector3d C2, Vector3d &point,
						Vector3d &normal) override;
};

#endif //FEM_FASTDCD_H
