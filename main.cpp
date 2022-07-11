#include "Simulator/Simulator.h"
#include "NumericSolver/LCPSolver/BGS.h"
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
#include "BodyEnergy/SoftBodyGravity.h"
#include "Output/FileOutput.h"
#include "json/json.h"
#include <iostream>

#include "App.h"

int main() {
	Json::Value root;
	std::fstream config(RESOURCE_PATH "/config/robot-arm.json");

	Json::CharReaderBuilder builder;
	builder["collectComments"] = false;
	JSONCPP_STRING err;

	if (!Json::parseFromStream(builder, config, &root, &err)) {
		spdlog::error("Error occurs while reading from json files: {}", err);
		exit(EXIT_FAILURE);
	}

	SimulatorParameter para(
			root.get("simulation-config", Json::nullValue).get("duration", 5).asDouble(),
			root.get("simulation-config", Json::nullValue).get("time-step", 0.01).asDouble(),
			SystemParameter(),
			IntegratorType::kStaggeringLCP,
			LCPIntegratorParameter(
					LCPSolverType::kOSQP,
					OSQPWrapperParameter (
							root.get("solver-config", Json::nullValue).get("max-iteration", 300).asInt(),
							root.get("solver-config", Json::nullValue).get("tolerance", 1e-3).asDouble()
					)
			),
			ContactGeneratorType::kDCD,
			DCDContactGeneratorParameter(
					DCDType::kFast,
					DCDParameter(
							root.get("solver-config", Json::nullValue).get("max-iteration", 300).asInt(),
							root.get("solver-config", Json::nullValue).get("tolerance", 1e-3).asDouble()
					)
			),
			FrictionModelType::kInscribedPolygon,
			PolygonFrictionModelParameter(
					root.get("friction-model", Json::nullValue).get("num-tangent", 4).asInt()
			),
			SimulatorOutputType::kFile,
			FileOutputParameter (
					RESOURCE_PATH + root.get("output-dir", Json::nullValue).asString()
			)
	);

	App app;

	app.Initialize(para);

	const auto& soft_bodies = root.get("softbodies", Json::nullValue);
	int num_soft_bodies = static_cast<int>(soft_bodies.size());

	double alpha1 = root.get("dissipation-model", Json::nullValue).get("alpha1", 1).asDouble();
	double alpha2 = root.get("dissipation-model", Json::nullValue).get("alpha2", 1).asDouble();

	for (int i = 0; i < num_soft_bodies; i++) {
		const auto& soft_body_config = soft_bodies[i];
		Mesh mesh;

		BodyEnergyParameter body_energy (
				ElasticEnergyModelType::kSimple,		// elastic energy model
				SimpleModelParameter(),
				DissipationEnergyModelType::kRayleigh,	// dissipation energy model
				RayleighModelParameter(
						alpha1,		// alpha1, for mass
						alpha2    	// alpha2, for K
				),
				ConstituteModelType::kStVK,
				StVKModelParameter(
						soft_body_config.get("youngs-module", 1e4).asDouble(),    // Young's module
						soft_body_config.get("poisson-ratio", 0.47).asDouble()    // Poisson's ratio
				)
		);

		SoftBodyParameter soft_para(
				soft_body_config.get("mu", 1.0).asDouble(),
				MassModelType::kVoronoi,
				VoronoiModelParameter(soft_body_config.get("density", 1.0).asDouble()),
				body_energy
		);
		mesh.Initialize(MeshParameter(RESOURCE_PATH + soft_body_config.get("src", "").asString()));
		SoftBody soft_body(mesh);
		soft_body.Initialize(soft_para);
		soft_body.AddExternalForce(SoftBodyGravity(9.8));
		app.AddObject(soft_body);
	}

	const auto& robot_arms = root.get("robot-arms", Json::nullValue);
	int num_robot_arm = static_cast<int>(robot_arms.size());

	for (int i = 0; i < num_robot_arm; i++) {
		auto robot_arm_config = robot_arms[i];
		double mu = robot_arm_config.get("mu", 0.5).asDouble();
		double density = robot_arm_config.get("density", 1).asDouble();
		double force = robot_arm_config.get("force", Json::nullValue).asDouble();
		Vector3d center, shape, euler, direction;
		center << robot_arm_config.get("x", Json::nullValue).asDouble(),
				  robot_arm_config.get("y", Json::nullValue).asDouble(),
				  robot_arm_config.get("z", Json::nullValue).asDouble();
		shape << robot_arm_config.get("length", Json::nullValue).asDouble(),
				 robot_arm_config.get("width", Json::nullValue).asDouble(),
				 robot_arm_config.get("height", Json::nullValue).asDouble();
		euler << robot_arm_config.get("phi", Json::nullValue).asDouble(),
				 robot_arm_config.get("theta", Json::nullValue).asDouble(),
				 robot_arm_config.get("psi", Json::nullValue).asDouble();
		direction << robot_arm_config.get("dir-x", Json::nullValue).asDouble(),
					 robot_arm_config.get("dir-y", Json::nullValue).asDouble(),
					 robot_arm_config.get("dir-z", Json::nullValue).asDouble();
		RobotArm robot_arm(mu, density, center, euler, shape, direction);
		robot_arm.AddExternalForce(RobotArmForce(direction, force));
		app.AddObject(robot_arm);
	}

	const auto& fixed_slabs = root.get("slabs", Json::nullValue);
	int num_fixed_slab = static_cast<int>(fixed_slabs.size());

	for (int i = 0; i < num_fixed_slab; i++) {
		const auto& fixed_slab_config = fixed_slabs[i];
		double mu = fixed_slab_config.get("mu", 0.5).asDouble();
		double density = fixed_slab_config.get("density", 1).asDouble();
		Vector3d center, shape, euler;
		center << fixed_slab_config.get("x", Json::nullValue).asDouble(),
				fixed_slab_config.get("y", Json::nullValue).asDouble(),
				fixed_slab_config.get("z", Json::nullValue).asDouble();
		shape << fixed_slab_config.get("length", Json::nullValue).asDouble(),
				fixed_slab_config.get("width", Json::nullValue).asDouble(),
				fixed_slab_config.get("height", Json::nullValue).asDouble();
		euler << fixed_slab_config.get("phi", Json::nullValue).asDouble(),
				fixed_slab_config.get("theta", Json::nullValue).asDouble(),
				fixed_slab_config.get("psi", Json::nullValue).asDouble();

		app.AddObject(FixedSlab(mu, density, center, euler, shape));
	}
//	app.Simulate();
	app.MainLoop();

	return 0;
}