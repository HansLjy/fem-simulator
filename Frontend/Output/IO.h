//
// Created by hansljy on 22-6-30.
//

#ifndef FEM_IO_H
#define FEM_IO_H

#include "System/System.h"
#include "Util/Pattern.h"
#include <string>

enum class SimulatorOutputType {
	kFile,
	kGUI,
};

class SimulatorOutputParameter {
public:
	SimulatorOutputParameter() = default;
	virtual ~SimulatorOutputParameter() = default;

	BASE_DECLARE_CLONE(SimulatorOutputParameter)
	DECLARE_VIRTUAL_ACCESSIBLE_MEMBER(std::string, OutputDir)
};

class SimulatorOutput {
public:
	virtual void Initialize(const SimulatorOutputParameter& para) = 0;
	virtual void StepCB(const System &system, int frame_id) = 0;

	virtual ~SimulatorOutput() = default;
};

#endif //FEM_IO_H
