#include "Mesh/Mesh.h"


int main() {
	MeshParameter para(
		"Test/vtk/bunny.vtk"
	);

	Mesh mesh;
	mesh.Initialize(para);
	mesh.Store("Test/output/bunny_out.vtk");
	return 0;
}
