//
// Created by hansljy on 22-6-30.
//

#include "IO.h"

class GUIOutputParameter : public SimulatorOutputParameter {
};

class GUIOutput : public SimulatorOutput {
public:
	void Initialize(const SimulatorOutputParameter &para) override;
	void StepCB(const System &system, int frame_id) override;

protected:

};