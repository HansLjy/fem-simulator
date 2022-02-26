#include "Mesh/Mesh.h"
#include "Mass/VoronoiModel.h"
#include "Util/Factory.h"
#include <iostream>

int main() {
	MeshParameter para(
		"../Test/vtk/two-tet.vtk"
	);

	Mesh mesh;
	mesh.Initialize(para);

	MassModel* mass_model = MassModelFactory::GetInstance()->GetMassModel(MassModelType::kVoronoi);
	auto mass_model_para = new MassModelParameter;
	mass_model->Initialize(*mass_model_para);
	auto mass = mass_model->GetMassDistribution(mesh);
	std::cout << mass;

	return 0;
}
