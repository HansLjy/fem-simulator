//
// Created by hansljy on 2022/2/22.
//

#ifndef FEM_FACTORY_H
#define FEM_FACTORY_H

#include "Pattern.h"

#include "ConstituteModel/ConstituteModel.h"
DECLARE_XXX_FACTORY(ConstituteModel)

#include "ElementEnergy/ElasticEnergy.h"
DECLARE_XXX_FACTORY(ElasticEnergyModel)

#include "ElementEnergy/DissipationEnergy.h"
DECLARE_XXX_FACTORY(DissipationEnergyModel)

#include "Mass/MassModel.h"
DECLARE_XXX_FACTORY(MassModel)

#include "NumericSolver/Optimizer/Optimizer.h"
DECLARE_XXX_FACTORY(Optimizer)

#include "NumericSolver/LCPSolver/LCPSolver.h"
DECLARE_XXX_FACTORY(LCPSolver)

#include "Integrator/Integrator.h"
DECLARE_XXX_FACTORY(Integrator)

#include "Contact/ContactGenerator.h"
DECLARE_XXX_FACTORY(ContactGenerator)

#include "Contact/FrictionModel.h"
DECLARE_XXX_FACTORY(FrictionModel)

#include "Contact/DCD/DCD.h"
DECLARE_XXX_FACTORY(DCD)

#include "Output/IO.h"
DECLARE_XXX_FACTORY(SimulatorOutput)

#endif //FEM_FACTORY_H
