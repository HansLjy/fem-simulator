//
// Created by hansljy on 22-5-6.
//

#include "PolygonFrictionModel.h"
#include <Eigen/CholmodSupport>
#include <spdlog/spdlog.h>

DEFINE_ACCESSIBLE_MEMBER(PolygonFrictionModelParameter, int, NumTangent, _num_tangent)
DEFINE_CLONE(FrictionModelParameter, PolygonFrictionModelParameter)

void PolygonFrictionModel::Initialize(const FrictionModelParameter &para) {
	_num_tangent = para.GetNumTangent();
	_sin.resize(_num_tangent);
	_cos.resize(_num_tangent);
	double angle = 0, delta_angle = 2 * EIGEN_PI / _num_tangent;
	for (int i = 0; i < _num_tangent; i++) {
		_cos[i] = cos(angle);
		_sin[i] = sin(angle);
		angle += delta_angle;
	}
}

void PolygonFrictionModel::GetJ(const System &system,
								const vector<ContactPoint> &contacts, double h,
								SparseMatrixXd &JnT, SparseMatrixXd &JtT,
								VectorXd &Mu) const {
	const auto& objects = system.GetObjects();
	const int num_contact = contacts.size();
	const int dof = system.GetSysDOF();
	JnT.resize(num_contact, dof), JtT.resize(num_contact * _num_tangent, dof);
	Mu.resize(contacts.size());

	COO coo_n, coo_t;
	int cur_num_contact = 0;
	for (auto& contact : contacts) {
		const auto& obj1 = objects[contact._obj1], obj2 = objects[contact._obj2];
		const Vector3d point = contact._point, normal = contact._normal;
		const int offset1 = system.GetOffset(contact._obj1), offset2 = system.GetOffset(contact._obj2);
		auto coo_n1 = obj1->GetJ(contact._type1, contact._idx1, point, -normal);
		auto coo_n2 = obj2->GetJ(contact._type2, contact._idx2, point, normal);
		for (auto& ele : coo_n1) {
			coo_n.push_back(Triplet(ele.row() + cur_num_contact, ele.col() + offset1, ele.value()));
		}
		for (auto& ele : coo_n2) {
			coo_n.push_back(Triplet(ele.row() + cur_num_contact, ele.col() + offset2, ele.value()));
		}

		Vector3d tangent1, tangent2;
		tangent1.x() = tangent1.y() = normal.z();
		tangent1.z() = - normal.x() - normal.y();
		assert(tangent1.dot(normal) < 1e-5);
		assert(tangent1.dot(normal) > -1e-5);
		tangent1 = (Vector3d)tangent1.normalized();
		tangent2 = tangent1.cross(normal);

		for (int i = 0; i < _num_tangent; i++) {
			Vector3d tangent = _cos[i] * tangent1 + _sin[i] * tangent2;
			auto coo_t1 = obj1->GetJ(contact._type1, contact._idx1, point, tangent);
			auto coo_t2 = obj1->GetJ(contact._type2, contact._idx2, point, tangent);
			for (auto& ele : coo_t1) {
				coo_t.push_back(Triplet(ele.row() + cur_num_contact * _num_tangent + i, ele.col() + offset1, ele.value()));
			}
			for (auto& ele : coo_t2) {
				coo_t.push_back(Triplet(ele.row() + cur_num_contact * _num_tangent + i, ele.col() + offset2, ele.value()));
			}
		}
		Mu(cur_num_contact) = contact._mu;
		cur_num_contact++;
	}
	JnT.setFromTriplets(coo_n.begin(), coo_n.end());
	JtT.setFromTriplets(coo_t.begin(), coo_t.end());
}

int PolygonFrictionModel::GetNumTangent() const {
	return _num_tangent;
}