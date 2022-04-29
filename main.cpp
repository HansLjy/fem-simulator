#include "Simulator/Simulator.h"
#include "NumericSolver/LCPSolver/BGS.h"
#include "NumericSolver/LCPSolver/PGS.h"
#include "NumericSolver/LCPSolver/PivotingMethod.h"
#include "NumericSolver/LCPSolver/OSQPWrapper.h"
#include "ElementEnergy/SimpleModel.h"
#include "ElementEnergy/RayleighModel.h"
#include "ConstituteModel/StVKModel.h"
#include "BodyEnergy/Gravity.h"
#include "Mass/VoronoiModel.h"
#include "BodyEnergy/GroundForce.h"
#include "Integrator/LCPIntegrator.h"
#include "Contact/PolyhedralCone.h"
#include "RigidBody/Rectangular.h"
#include "Util/Factory.h"
#include <iostream>
#include <fstream>

int main() {
	string output_dir;
	double duration, step;

	int max_step;
	double max_error, lambda;

	int num_tangent;

	double alpha1, alpha2;
	double youngs_module, poisson_ratio, density;

	std::fstream cfg("./config");
	cfg >> output_dir;
	cfg >> duration >> step;
	cfg >> max_error >> max_step >> lambda;
	cfg >> num_tangent;
	cfg >> alpha1 >> alpha2;
	cfg >> youngs_module >> poisson_ratio;
	cfg >> density;

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
		ContactGeneratorType::kPolyhedralCone,
		PolyhedralConeParameter (
			num_tangent
		),
		BodyEnergyParameter(
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
		)
	);

	simulator->Initialize(para);

	MassModel* mass_model = MassModelFactory::GetInstance()->GetMassModel(MassModelType::kVoronoi);
	mass_model->Initialize(VoronoiModelParameter(density));

	int num_soft_bodies;
	cfg >> num_soft_bodies;
	for (int i = 0; i < num_soft_bodies; i++) {
		Mesh soft_body;
		std::string input_file;
		cfg >> input_file;
		soft_body.Initialize(MeshParameter(input_file));
		simulator->AddSoftBody(soft_body, *mass_model);
	}

	int num_rectangles;
	cfg >> num_rectangles;
	for (int i = 0; i < num_rectangles; i++) {
		double x, y, z;
		double length, width, height;
		double theta, phi, psi;
		double mu;
		cfg >> x >> y >> z;
		cfg >> length >> width >> height;
		cfg >> phi >> theta >> psi;
		cfg >> mu;
		Vector3d center, shape, euler;
		center << x, y, z;
		shape << length, width, height;
		euler << phi, theta, psi;

		simulator->AddRigidBody(Rectangular(mu, center, euler, shape));
	}

	simulator->Initialize(para);
//	simulator->AddExternalForce(GroundForce(10));
	simulator->AddExternalForce(Gravity(9.8));

	simulator->Simulate();
	return 0;
}