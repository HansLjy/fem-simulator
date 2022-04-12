//
// Created by hansljy on 2022/4/9.
//

#ifndef FEM_SYSTEM_H
#define FEM_SYSTEM_H

#include <vector>
#include "Mesh/Mesh.h"
#include "BodyEnergy/ExternalForce.h"
#include "Mass/MassModel.h"
#include "SoftBody/SoftBody.h"
#include "RigidBody/RigidBody.h"

class SystemParameter {
	SystemParameter(const std::vector<std::string> input_files, const std::vector<MassModelType> mass_models) : _input_files(input_files), _mass_model_types(mass_models) {}

	DECLARE_ACCESSIBLE_MEMBER(std::vector<std::string>, InputFiles, _input_files)
	DECLARE_ACCESSIBLE_MEMBER(std::vector<MassModelType>, MassModelTypes, _mass_model_types);

public:
	DERIVED_DECLARE_CLONE(SystemParameter)
};

class System {
public:
	void Initialize(const SystemParameter& para);

	void AddSoftBody(const SoftBody& soft_body) {
		_soft_bodies.push_back(soft_body);
	}

	void AddRigidBody(const RigidBody& rigid_body) {
		_rigid_bodies.push_back(rigid_body);
	}

	void AddExternalForce(const ExternalForce& external_force) {
		_external_forces.push_back(external_force.Clone());
	}

	const std::vector<SoftBody>& GetSoftBodies() const {
		return _soft_bodies;
	}

	std::vector<SoftBody>& GetSoftBodies() {
		return _soft_bodies;
	}

	const std::vector<RigidBody>& GetRigidBodies() const {
		return _rigid_bodies;
	}

	const std::vector<const ExternalForce*>& GetExternalForces() const {
		return _external_forces;
	}

	void Store(const std::string& path, int frame_id) {
		int index = 0;
		for (auto& soft_body : _soft_bodies) {
			soft_body._mesh.Store(path + "/soft_obj" + std::to_string(index++) + "f" + std::to_string(frame_id) + ".vtk");
		}
	}

private:
	std::vector<const ExternalForce*> _external_forces;
	std::vector<SoftBody> _soft_bodies;
	std::vector<RigidBody> _rigid_bodies;
};

#endif //FEM_SYSTEM_H
