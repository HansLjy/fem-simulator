//
// Created by hansljy on 22-6-30.
//

#ifndef FEM_GUIOUTPUT_H
#define FEM_GUIOUTPUT_H

#include "IO.h"
#include <mutex>
#include <vector>
#include <queue>
#include <condition_variable>

#define POOL_SIZE 10

class GUIOutputParameter : public SimulatorOutputParameter {
public:
	DERIVED_DECLARE_CLONE(SimulatorOutputParameter)
};

class GUIOutput : public SimulatorOutput {
public:
	void Initialize(const SimulatorOutputParameter &para) override;
	void StepCB(const System &system, int frame_id) override;

protected:
	double _render_step;
	std::mutex _mtx;
	std::condition_variable_any _cv;

	std::queue<std::vector<MatrixXd>> _vertices;
	std::vector<std::vector<Matrix<int, Dynamic, 3>>> _topos;
};

#endif //FEM_GUIOUTPUT_H
