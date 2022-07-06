//
// Created by hansljy on 22-6-30.
//

#ifndef FEM_FILEOUTPUT_H
#define FEM_FILEOUTPUT_H

#include "IO.h"

class FileOutputParameter : public SimulatorOutputParameter {
public:
	FileOutputParameter(const std::string& output_dir);
	~FileOutputParameter() override = default;
	DERIVED_DECLARE_CLONE(SimulatorOutputParameter)
	DECLARE_OVERWRITE_ACCESSIBLE_MEMBER(std::string, OutputDir, _output_dir)
};

class FileOutput : public SimulatorOutput {
public:
	void Initialize(const SimulatorOutputParameter & para) override;
	void StepCB(const System &system, int frame_id) override;

protected:
	std::string _output_dir;
};

#endif //FEM_FILEOUTPUT_H
