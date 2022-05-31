//
// Created by hansljy on 2022/4/9.
//

#ifndef FEM_SYSTEM_H
#define FEM_SYSTEM_H

#include <vector>
#include "Mesh/Mesh.h"
#include "BodyEnergy/ExternalForce.h"
#include "Mass/MassModel.h"
#include "Object/SoftBody/SoftBody.h"
#include "Object/RigidBody/RigidBody.h"
#include "BodyEnergy/BodyEnergy.h"

class SystemParameter {
public:
	DERIVED_DECLARE_CLONE(SystemParameter)
};

class System {
public:
	void Initialize(const SystemParameter& para) {}

	const std::vector<Object*>& GetObjects() const {
		return _objects;
	}

	int AddObject(const Object& object) {
		_objects.push_back(object.Clone());
		return _objects.size() - 1;
	}

	void RemoveObject(int obj_idx) {
		if (obj_idx >= _objects.size()) {
			return;
		} else {
			_objects.erase(_objects.begin() + obj_idx);
		}
	}

	const Object& GetObject(int obj_idx) {
		return *_objects[obj_idx];
	}

	void Store(const std::string& path, int frame_id) const {
		int index = 0;
		for (auto& object : _objects) {
			object->Store(path + "/object" + std::to_string(index++) + "f" + std::to_string(frame_id) + ".vtk");
		}
	}

	int GetSysDOF() const {
		return _dof;
	}

	const SparseMatrixXd& GetSysMass() const {
		return _mass;
	}

	void GetSysV(VectorXd& v) const {
		v.resize(_dof);
		const int num_objects = _objects.size();
		for (int i = 0; i < num_objects; i++) {
			v.block(_dof_offsets[i], 0, _objects[i]->GetDOF(), 1) = _objects[i]->GetV();
		}
	}

	void GetSysX(VectorXd& x) const {
		x.resize(_dof);
		const int num_objects = _objects.size();
		for (int i = 0; i < num_objects; i++) {
			x.block(_dof_offsets[i], 0, _objects[i]->GetDOF(), 1) = _objects[i]->GetX();
		}
	}

	// TODO: ADD DISSIPATION FORCE INTO CALCULATION!
	// dissipation forces are implemented but not used for various reasons

	void GetSysF(VectorXd &f) const {
		f.resize(_dof);
		const int num_objects = _objects.size();
		for (int i = 0; i < num_objects; i++) {
			const int dof_offset = _dof_offsets[i];
			const int single_dof = _objects[i]->GetDOF();
			f.block(dof_offset, 0, single_dof, 1) = -_objects[i]->EnergyGradient();
		}
	}

	void GetSysEnergyHessian(SparseMatrixXd &hessian) const {
		const int num_objects = _objects.size();
		hessian.resize(_dof, _dof);
		std::vector<Triplet> COO;
		for (int i = 0; i < num_objects; i++) {
			auto single_hession = _objects[i]->EnergyHessian();
			const int offset = _dof_offsets[i];
			for (int j = 0; j < single_hession.outerSize(); j++) {
				for (SparseMatrixXd::InnerIterator it(single_hession, j); it; ++it) {
					COO.push_back(Triplet(it.row() + offset, it.col() + offset, it.value()));
				}
			}
		}
		hessian.setFromTriplets(COO.begin(), COO.end());
	}

	int GetOffset(int idx) const {
		return _dof_offsets[idx];
	}

	void UpdateSettings() {
		_dof_offsets.clear();
		_dof = 0;
		for (auto& object : _objects) {
			_dof_offsets.push_back(_dof);
			_dof += object->GetDOF();
		}
		_mass.resize(_dof, _dof);
		COO coo;
		const int num_objects = _objects.size();
		for (int i = 0; i < num_objects; i++) {
			auto& object = _objects[i];
			auto single_coo = object->GetM();
			for (auto& ele : single_coo) {
				coo.push_back(Triplet(ele.row() + _dof_offsets[i],
									  ele.col() + _dof_offsets[i],
									  ele.value()));
			}
		}
		_mass.setFromTriplets(coo.begin(), coo.end());
	}

	void UpdateDynamic(const VectorXd& u, double h) {
		const int num_objects = _objects.size();
		for (int i = 0; i < num_objects; i++) {
			_objects[i]->GetX() += u.block(_dof_offsets[i], 0, _objects[i]->GetDOF(), 1) * h;
			_objects[i]->GetV() = u.block(_dof_offsets[i], 0, _objects[i]->GetDOF(), 1);
		}
	}

	virtual ~System() {
		for (auto& object : _objects) {
			delete object;
		}
	}

private:
	std::vector<Object*> _objects;
	std::vector<int> _dof_offsets;
	SparseMatrixXd _mass;
	int _dof;
};

#endif //FEM_SYSTEM_H
