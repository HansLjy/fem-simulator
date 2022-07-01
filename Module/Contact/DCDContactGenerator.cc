//
// Created by hansljy on 22-5-20.
//

#include "DCDContactGenerator.h"
#include "Util/Pattern.h"
#include "Util/Factory.h"

DEFINE_ACCESSIBLE_MEMBER(DCDContactGeneratorParameter, DCDType, DCDType, _dcd_type)
DEFINE_ACCESSIBLE_POINTER_MEMBER(DCDContactGeneratorParameter, DCDParameter, DCDPara, _dcd_parameter)
DEFINE_CLONE(ContactGeneratorParameter, DCDContactGeneratorParameter)

void DCDContactGenerator::Initialize(const ContactGeneratorParameter &para) {
	_dcd = DCDFactory::GetInstance()->GetDCD(para.GetDCDType());
	_dcd->Initialize(*para.GetDCDPara());
}

void DCDContactGenerator::GetContact(const System &system,
									 vector<ContactPoint> &contact_points) const {
	contact_points.clear();
	const auto &objects = system.GetObjects();
	const int num_objs = objects.size();
	for (int i1 = 0; i1 < num_objs; i1++) {
		const auto &object1 = objects[i1];
		const auto &surface_converter1 = object1->GetDOFShapeConverter();
		const auto &surface_vertices1 = surface_converter1->GetSurfacePosition(*object1);
		const auto &surface_topo1 = surface_converter1->GetSurfaceTopo(*object1);
		for (int i2 = 0; i2 < i1; i2++) {
			const auto &object2 = objects[i2];
			const auto &surface_converter2 = object2->GetDOFShapeConverter();
			const auto &surface_vertices2 = surface_converter2->GetSurfacePosition(*object2);
			const auto &surface_topo2 = surface_converter2->GetSurfaceTopo(*object2);

			int num_face1 = surface_topo1.rows(),
				num_face2 = surface_topo2.rows();
			for (int face_id1 = 0; face_id1 < num_face1; face_id1++) {
				for (int face_id2 = 0; face_id2 < num_face2; face_id2++) {
					Vector3d point, normal;
					const auto& face_elements_id1 = surface_topo1.row(face_id1);
					const auto& face_elements_id2 = surface_topo2.row(face_id2);
					bool intersected = _dcd->GetIntersected(
						surface_vertices1.row(face_elements_id1(0)),
						surface_vertices1.row(face_elements_id1(1)),
						surface_vertices1.row(face_elements_id1(2)),
						surface_vertices2.row(face_elements_id2(0)),
						surface_vertices2.row(face_elements_id2(1)),
						surface_vertices2.row(face_elements_id2(2)),
						point, normal);
					if (intersected) {
						contact_points.push_back(ContactPoint(
							i1, i2,
							face_id1, face_id2,
							point, normal
						));
					}
				}
			}
		}
	}
}