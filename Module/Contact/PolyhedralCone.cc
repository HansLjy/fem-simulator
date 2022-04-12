//
// Created by hansljy on 2022/4/9.
//

#include "PolyhedralCone.h"
#include <cmath>

const double pi = acos(-1);

void PolyhedralCone::GetContact(const System &system, SparseMatrixXd &Jn,
								SparseMatrixXd &Jt,
								VectorXd &mu) const {
	const auto& soft_bodies = system.GetSoftBodies();
	const auto& rigid_bodies = system.GetRigidBodies();
	int num_soft = soft_bodies.size();
	int num_rigid = rigid_bodies.size();

	std::vector<Triplet> triplets_n;
	std::vector<Triplet> triplets_t;

	int node_id = 0;		// id of the node in the whole system
	int num_contact = 0;	// number of contact points

	double cosine[_num_tangent];
	double sine[_num_tangent];

	double angle = 0, delta_angle = 2 * pi / _num_tangent;
	for (int i = 0; i < _num_tangent; i++) {
		cosine[i] = cos(angle);
		sine[i] = sin(angle);
		angle += delta_angle;
	}

	std::vector<double> mu_vec;

	for (int soft_id = 0; soft_id < num_soft; soft_id++) {
		const auto& points = soft_bodies[soft_id]._mesh.GetPoints();
		int num_nodes = points.size() / 3;
		for (int i = 0; i < num_nodes; i++, node_id++) {
			Vector3d X = points.block<3, 1>(3 * i, 0);
			for (int rigid_id = 0; rigid_id < num_rigid; rigid_id++) {
				if (rigid_bodies[rigid_id].Distance(X) < 0) {
					Vector3d normal = rigid_bodies[rigid_id].Normal(X);
					for (int j = 0; j < 3; j++) {
						triplets_n.push_back(Triplet(num_contact, 3 * node_id + j, normal(j)));
					}
					Vector3d tangent1, tangent2;
					tangent1.x() = tangent1.y() = normal.z();
					tangent1.z() = - normal.x() - normal.y();
					assert(tangent1.dot(normal) < 1e-5);
					assert(tangent1.dot(normal) > -1e-5);
					tangent1 = (Vector3d)tangent1.normalized();
					tangent2 = tangent1.cross(normal);

					for (int j = 0; j < _num_tangent; j++) {
						Vector3d tangent = cosine[j] * tangent1 + sine[j] * tangent2;
						int line_num = num_contact * _num_tangent + j;
						for (int k = 0; k < 3; k++) {
							triplets_t.push_back(Triplet(line_num, 3 * node_id + k, tangent(k)));
						}
					}

					mu_vec.push_back(rigid_bodies[rigid_id].GetMu());
					num_contact++;
				}
			}
		}
	}

	Jn.resize(num_contact, node_id * 3);
	Jn.setFromTriplets(triplets_n.begin(), triplets_n.end());

	Jt.resize(num_contact * _num_tangent, node_id * 3);
	Jt.setFromTriplets(triplets_t.begin(), triplets_t.end());

	mu.resize(num_contact);
	for (int i = 0; i < num_contact; i++) {
		mu(i) = mu_vec[i];
	}
}