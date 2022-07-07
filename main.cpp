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
#include <iostream>

#include "App.h"

int main() {
	string output_dir;
	double duration, step;

	int max_step;
	double max_error;

	int num_tangent;

	double alpha1, alpha2;

	std::fstream cfg("./config");
	cfg >> output_dir;
	cfg >> duration >> step;
	cfg >> max_error >> max_step;
	cfg >> num_tangent;
	cfg >> alpha1 >> alpha2;

	SimulatorParameter para(
			duration,					// duration
			step,						// step
			SystemParameter(),
			IntegratorType::kStaggeringLCP,
			LCPIntegratorParameter(
					LCPSolverType::kOSQP,
					OSQPWrapperParameter (
							max_step,
							max_error
					)
			),
			ContactGeneratorType::kDCD,
			DCDContactGeneratorParameter(
					DCDType::kFast,
					DCDParameter(
							max_step,	// max iteration
							1e-6	// tolerance
					)
			),
			FrictionModelType::kInscribedPolygon,
			PolygonFrictionModelParameter(
					num_tangent
			),
			SimulatorOutputType::kFile,
			FileOutputParameter (
					output_dir
			)
	);

	App app;

	app.Initialize(para);

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
		app.AddObject(soft_body);
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
		app.AddObject(robot_arm);
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
		app.AddObject(FixedSlab(mu, density, center, euler, shape));
	}
	app.MainLoop();

	return 0;
}