//
// Created by hansljy on 22-7-1.
//

#include "GUI.h"
#include <igl/opengl/glfw/Viewer.h>
#include <igl/opengl/glfw/imgui/ImGuiMenu.h>
#include <igl/opengl/glfw/imgui/ImGuiPlugin.h>
#include <igl/opengl/glfw/imgui/ImGuiHelpers.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include "Simulator/Simulator.h"
#include "NumericSolver/LCPSolver/OSQPWrapper.h"
#include "ElementEnergy/SimpleModel.h"
#include "ElementEnergy/RayleighModel.h"
#include "ConstituteModel/StVKModel.h"
#include "Mass/VoronoiModel.h"
#include "Integrator/LCPIntegrator.h"
#include "Contact/DCDContactGenerator.h"
#include "Contact/PolygonFrictionModel.h"
#include "Object/RigidBody/RobotArm.h"
#include "Object/RigidBody/FixedSlab.h"
#include "BodyEnergy/RobotArmForce.h"
#include "Object/Object.h"
#include "BodyEnergy/SoftBodyGravity.h"
#include "Util/Factory.h"
#include "Output/GUIOutput.h"
#include <thread>

void GUI::Start() {
	igl::opengl::glfw::imgui::ImGuiPlugin plugin;
	_viewer.plugins.push_back(&plugin);
	igl::opengl::glfw::imgui::ImGuiMenu menu;
	plugin.widgets.push_back(&menu);

	menu.callback_draw_viewer_menu = [&] () {
		if (ImGui::CollapsingHeader("Menu", ImGuiTreeNodeFlags_DefaultOpen)) {
			if (ImGui::Button("Start")) {
				ButtonStartCB();
			}
			if (ImGui::Button("Stop")) {
				ButtonStopCB();
			}
			if (ImGui::Button("Quit")) {
				ButtonQuitCB();
			}
			if (ImGui::Button("Continue")) {
				ButtonContinueCB();
			}
			ImGui::InputInt("Render Step", &_render_step);
			if (ImGui::Button("Change Configuration")) {
				ChangeConfigCB();
			}
			ImGui::InputText("Scene config file", _scene_file);
			if (ImGui::Button("Load Scene")) {
				LoadSceneCB();
			}
		}
	};

	_viewer.launch();
}

void GUI::SimulationLoop() {
	int index = 0;
	while(true) {
		_mtx.lock();
		_output->StepCB(*_system, index);
		_integrator->Step(*_system, *_contact, *_friction, _simulation_step);
		_mtx.unlock();
	}
}

void GUI::RenderLoop() {
	while(true) {
		_mtx.lock();
		const auto& objects = _system->GetObjects();
		_viewer.selected_data_index = 0;
		for (const auto& object : objects) {
			const auto &converter = object->GetDOFShapeConverter();
			_viewer.data().clear();
			_viewer.data().set_mesh(converter->GetSurfacePosition(*object),
									converter->GetSurfaceTopo(*object));
			_viewer.selected_data_index++;
		}
		_mtx.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(_render_step));
	}
}



void GUI::ButtonStartCB() {
	std::string output_dir;

	int max_step;
	double max_error;

	int num_tangent;

	double alpha1, alpha2;

	std::fstream cfg(_scene_file);
	if (cfg.bad()) {
		spdlog::error("Cannot find configuration file");
		return;
	}
	cfg >> output_dir;
	cfg >> _simulation_step;
	cfg >> max_error >> max_step;
	cfg >> num_tangent;
	cfg >> alpha1 >> alpha2;

	delete _system;
	_system = new System();
	_system->Initialize(SystemParameter());

	spdlog::info("System initialized");

	delete _integrator;
	_integrator = IntegratorFactory::GetInstance()->GetIntegrator(
			IntegratorType::kStaggeringLCP);
	_integrator->Initialize(
			LCPIntegratorParameter(
					LCPSolverType::kOSQP,
					OSQPWrapperParameter(
							max_step,
							max_error
					)
			)
	);

	spdlog::info("Integrator initialized");

	delete _contact;
	_contact = ContactGeneratorFactory::GetInstance()->GetContactGenerator(
			ContactGeneratorType::kDCD);
	_contact->Initialize(
			DCDContactGeneratorParameter(
					DCDType::kFast,
					DCDParameter(
							max_step,    // max iteration
							1e-6    // tolerance
					)
			)
	);

	spdlog::info("Contact initialized");

	delete _friction;
	_friction = FrictionModelFactory::GetInstance()->GetFrictionModel(
			FrictionModelType::kInscribedPolygon);
	_friction->Initialize(
			PolygonFrictionModelParameter(
					num_tangent
			));

	spdlog::info("Friction initialized");

	delete _output;
	_output = SimulatorOutputFactory::GetInstance()->GetSimulatorOutput(
			SimulatorOutputType::kGUI);
	_output->Initialize(GUIOutputParameter());

	spdlog::info("Output initialized");

	int num_soft_bodies;
	cfg >> num_soft_bodies;
	for (int i = 0; i < num_soft_bodies; i++) {
		Mesh mesh;
		std::string input_file;
		double density;
		double mu_soft;
		double youngs_module, poisson_ratio;

		cfg >> input_file;
		cfg >> density;
		cfg >> mu_soft;
		cfg >> youngs_module >> poisson_ratio;
		BodyEnergyParameter body_energy(
				ElasticEnergyModelType::kSimple,        // elastic energy model
				SimpleModelParameter(),
				DissipationEnergyModelType::kRayleigh,    // dissipation energy model
				RayleighModelParameter(
						alpha1,        // alpha1, for mass
						alpha2        // alpha2, for K
				),
				ConstituteModelType::kStVK,
				StVKModelParameter(
						youngs_module,    // Young's module
						poisson_ratio    // Poisson's ratio
				)
		);

		SoftBodyParameter soft_para(
				mu_soft,
				MassModelType::kVoronoi,
				VoronoiModelParameter(density),
				body_energy
		);
		mesh.Initialize(MeshParameter(input_file));
		SoftBody soft_body(mesh);
		soft_body.Initialize(soft_para);
		soft_body.AddExternalForce(SoftBodyGravity(9.8));
		_system->AddObject(soft_body);
	}

	int num_robot_arm;
	cfg >> num_robot_arm;
	for (int i = 0; i < num_robot_arm; i++) {
		double mu, density;
		double x, y, z;
		double length, width, height;
		double theta, phi, psi;
		double dir_x, dir_y, dir_z;
		double force;
		cfg >> mu >> density;
		cfg >> x >> y >> z;
		cfg >> length >> width >> height;
		cfg >> phi >> theta >> psi;
		cfg >> dir_x >> dir_y >> dir_z;
		cfg >> force;

		Vector3d center, shape, euler;
		center << x, y, z;
		shape << length, width, height;
		euler << phi, theta, psi;
		Vector3d direction;
		direction << dir_x, dir_y, dir_z;
		RobotArm robot_arm(mu, density, center, euler, shape, direction);
		robot_arm.AddExternalForce(RobotArmForce(direction, force));
		_system->AddObject(robot_arm);
	}

	int num_fixed_slab;
	cfg >> num_fixed_slab;
	for (int i = 0; i < num_fixed_slab; i++) {
		double mu, density;
		double x, y, z;
		double length, width, height;
		double theta, phi, psi;
		double dir_x, dir_y, dir_z;
		cfg >> mu >> density;
		cfg >> x >> y >> z;
		cfg >> length >> width >> height;
		cfg >> phi >> theta >> psi;
		cfg >> dir_x >> dir_y >> dir_z;

		Vector3d center, shape, euler;
		center << x, y, z;
		shape << length, width, height;
		euler << phi, theta, psi;
		_system->AddObject(FixedSlab(mu, density, center, euler, shape));
	}


	_system->UpdateSettings();
	const auto objects = _system->GetObjects();
	for (const auto& object : objects) {
		_viewer.append_mesh();
		const auto &converter = object->GetDOFShapeConverter();
		_viewer.data().set_mesh(converter->GetSurfacePosition(*object),
								converter->GetSurfaceTopo(*object));
	}

	spdlog::info("Start loop");

	std::thread simulation_thread(&GUI::SimulationLoop, this);
	simulation_thread.detach();

	std::thread render_thread(&GUI::RenderLoop, this);
	render_thread.detach();
}

void GUI::ButtonStopCB() {
	spdlog::info("Stop");
}

void GUI::ButtonContinueCB() {
	spdlog::info("Continue");
}

void GUI::ButtonQuitCB() {
	spdlog::info("Quit");
}

void GUI::LoadSceneCB() {
	spdlog::info("Load Scene");
}

void GUI::ChangeConfigCB() {
	spdlog::info("Change Config");
}


