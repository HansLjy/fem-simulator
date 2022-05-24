#include "Simulator/Simulator.h"
#include "NumericSolver/LCPSolver/BGS.h"
#include "NumericSolver/LCPSolver/PGS.h"
#include "NumericSolver/LCPSolver/PivotingMethod.h"
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
#include "Object/Object.h"
#include "BodyEnergy/SoftBodyGravity.h"
#include "Util/Factory.h"
#include <iostream>
#include <fstream>

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

	auto simulator = new Simulator;
	SimulatorParameter para(
		duration,					// duration
		step,						// step
		output_dir,					// output
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
		)
	);

	simulator->Initialize(para);

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
		soft_body.AddExternalForce(SoftBodyGravity(soft_body, 9.8));
		simulator->AddObject(soft_body);
	}

	int num_rectangles;
	cfg >> num_rectangles;
	for (int i = 0; i < num_rectangles; i++) {
		double mu, density;
		double x, y, z;
		double length, width, height;
		double theta, phi, psi;
		cfg >> mu >> density;
		cfg >> x >> y >> z;
		cfg >> length >> width >> height;
		cfg >> phi >> theta >> psi;
		Vector3d center, shape, euler;
		center << x, y, z;
		shape << length, width, height;
		euler << phi, theta, psi;

		simulator->AddObject(FixedSlab(mu, density, center, euler, shape));
	}
	simulator->Simulate();
	return 0;
}