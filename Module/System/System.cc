//
// Created by hansljy on 2022/4/9.
//

#include "System.h"
#include "Util/Factory.h"

DEFINE_ACCESSIBLE_MEMBER(SystemParameter, std::vector<std::string>, InputFiles, _input_files)
DEFINE_ACCESSIBLE_MEMBER(SystemParameter, std::vector<MassModelType>, MassModelTypes, _mass_model_types)
DEFINE_CLONE(SystemParameter, SystemParameter)

void System::Initialize(const SystemParameter& para) {
	const auto& input_files = para.GetInputFiles();
	const auto& mass_model_types = para.GetMassModelTypes();
	int size = input_files.size();
	for (int i = 0; i < size; i++) {
		Mesh new_mesh;
		new_mesh.Initialize(MeshParameter(input_files[i]));
		MassModel *mass_model = MassModelFactory::GetInstance()->GetMassModel(
				mass_model_types[i]);
		_soft_bodies.push_back(SoftBody(new_mesh, *mass_model));
	}
}