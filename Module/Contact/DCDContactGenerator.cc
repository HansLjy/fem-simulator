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
		const auto &surface1 = object1->GetSurface();
		for (int i2 = 0; i2 < i1; i2++) {
			const auto &object2 = objects[i2];
			const auto &surface2 = object2->GetSurface();
			int num_face1 = surface1->GetNumFaces(),
				num_face2 = surface2->GetNumFaces();
			for (int face_id1 = 0; face_id1 < num_face1; face_id1++) {
				for (int face_id2 = 0; face_id2 < num_face2; face_id2++) {
					Vector3d point, normal;
					bool intersected = _dcd->GetIntersected(
							surface1->GetFace(face_id1),
							surface2->GetFace(face_id2), point, normal);
					if (intersected) {
						contact_points.push_back(ContactPoint(
								i1, i2,
								SurfaceElements::SurfaceType::kFace,
								SurfaceElements::SurfaceType::kFace,
								face_id1, face_id2,
								point, normal,
								std::max(object1->GetMu(), object2->GetMu())
						));
					}
				}
			}
		}
	}
}