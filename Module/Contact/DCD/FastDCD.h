//
// Created by hansljy on 22-5-20.
//

#ifndef FEM_FASTDCD_H
#define FEM_FASTDCD_H

#include "DCD.h"

class FastDCD : public DCD {
	void Initialize(const DCDParameter &para) override;
	bool GetIntersected(const SurfaceElements::Face &face1,
						const SurfaceElements::Face &face2, Vector3d &point,
						Vector3d &normal) override;
};

#endif //FEM_FASTDCD_H
