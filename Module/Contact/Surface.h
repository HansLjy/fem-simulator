//
// Created by hansljy on 22-5-6.
//

#ifndef FEM_SURFACE_H
#define FEM_SURFACE_H

#include <vector>
#include "Util/EigenAll.h"

namespace SurfaceElements {
	enum class SurfaceType {
		kFace,
		kEdge,
		kVertex
	};

	// The normal of the face can be determined as:
	// (v1 - v0) x (v2 - v0)
	struct Face {
		Vector3d _vertex[3];
	};

	struct Edge {
		Vector3d _vectex[2];
	};

	struct Vertex {
		Vector3d _vertex;
	};
}

class Surface {
public:
	Surface() = default;

	virtual int GetNumFaces() const = 0;
	virtual SurfaceElements::Face GetFace(int idx) const = 0;		// Get the faces on the surface

//	virtual int GetNumEdges() const = 0;
//	virtual SurfaceElements::Vertex GetEdge(int idx) const = 0;
//
//	virtual int GetNumVertices() const = 0;
//	virtual SurfaceElements::Vertex GetVertex(int idx) const = 0;

	virtual ~Surface() = default;
};

#endif //FEM_SURFACE_H
