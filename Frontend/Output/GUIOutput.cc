//
// Created by hansljy on 22-6-30.
//

#include "GUIOutput.h"

DEFINE_CLONE(SimulatorOutputParameter, GUIOutputParameter)

void GUIOutput::Initialize(const SimulatorOutputParameter &para) {}

void GUIOutput::StepCB(const System &system, int frame_id) {
	//
//	_cv.wait(_mtx, [this]{return _vertices.size() < POOL_SIZE;});
//	_mtx.lock();
//
//	std::vector<MatrixXd> cur_vertices;
//	std::vector<Matrix<int, Dynamic, 3>> cur_topos;
//
//	const auto& objects = system.GetObjects();
//	for (const auto& object : objects) {
//		const auto& converter = object->GetDOFShapeConverter();
//		cur_vertices.push_back(converter->GetSurfacePosition(*object));
//		cur_topos.push_back(converter->GetSurfaceTopo(*object));
//	}
//
//	_vertices.push(cur_vertices);
//	_topos.push_back(cur_topos);
//
//	_mtx.unlock();
//	_cv.notify_one();
}