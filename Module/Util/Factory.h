//
// Created by hansljy on 2022/2/22.
//

#ifndef FEM_FACTORY_H
#define FEM_FACTORY_H

#include "Pattern.h"

#include "ConsistencyModel/ConsitencyModel.h"
DECLARE_XXX_FACTORY(ConsistencyModel)

#include "ElementEnergy/ElasticEnergy.h"
DECLARE_XXX_FACTORY(ElasticEnergyModel)

#include "ElementEnergy/DissipationEnergy.h"
DECLARE_XXX_FACTORY(DissipationEnergyModel)

#include "Mass/MassModel.h"
DECLARE_XXX_FACTORY(MassModel)

#include "Optimizer/Optimizer.h"
DECLARE_XXX_FACTORY(Optimizer)

#include "Solver/LCPSolver/LCPSolver.h"
DECLARE_XXX_FACTORY(LCPSolver)

#include "Integrator/Integrator.h"
DECLARE_XXX_FACTORY(Integrator)

#include "Contact/ContactGenerator.h"
DECLARE_XXX_FACTORY(ContactGenerator)

#endif //FEM_FACTORY_H
