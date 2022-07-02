//
// Created by hansljy on 22-7-1.
//

#ifndef FEM_GUI_H
#define FEM_GUI_H

#include "Simulator/Simulator.h"
#include "igl/opengl/glfw/Viewer.h"
#include <condition_variable>
#include <mutex>
#include <atomic>

/**
 * Activities
 * 	1. Load configuration
 * 	2. Set render time step
 * 	3. Press start
 * 	4. Press stop
 * 	5. Press continue
 * 	6. Quit
 */

#include <string>

class GUI {
public:
	void Start();
	virtual void ButtonStartCB();
	virtual void ButtonStopCB();
	virtual void ButtonContinueCB();
	virtual void ButtonQuitCB();
	virtual void LoadSceneCB();
	virtual void ChangeConfigCB();
	virtual void ResetCB();

protected:
	std::string _scene_file = "./config";
	int _render_step = 10;

	double _simulation_step;			// time step
	System* _system = nullptr;			// the physical system

	Integrator* _integrator = nullptr;
	ContactGenerator* _contact = nullptr;
	FrictionModel* _friction = nullptr;
	SimulatorOutput* _output = nullptr;

	igl::opengl::glfw::Viewer _viewer;

protected:
	virtual void SimulationLoop();
	virtual void RenderLoop();
	std::mutex _mtx;
	std::mutex _stop_lock;
	std::condition_variable_any _stop_cv;
	bool _stopped = false;
	std::atomic<bool> _reset = false;
};

#endif //FEM_GUI_H
