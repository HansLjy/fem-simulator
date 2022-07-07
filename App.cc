//
// Created by hansljy on 22-7-6.
//

#include "App.h"

void App::InitializeScene(Scene &scene) {
	_current = 0;
	for (const auto& object : _system.GetObjects()) {
		scene.AddMesh();
		scene.SetMesh(object->GetSurfacePosition(), object->GetSurfaceTopo());
	}
	_system.UpdateSettings();
}

void App::Processing(Scene &scene) {
	_current += _step;
	if (_current > _duration) {
		glfwSetWindowShouldClose(GUI::window, 1);
	}
	_integrator->Step(_system, *_contact, *_friction, _step);
	int idx = 0;
	for (const auto& object : _system.GetObjects()) {
		scene.SelectData(idx++);
		scene.SetMesh(object->GetSurfacePosition(), object->GetSurfaceTopo());
	}
}