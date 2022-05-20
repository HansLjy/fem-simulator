//
// Created by hansljy on 22-5-20.
//

#ifndef FEM_DCDCONTACTGENERATOR_H
#define FEM_DCDCONTACTGENERATOR_H

#include "ContactGenerator.h"
#include "DCD/DCD.h"

class DCDContactGeneratorParameter : public ContactGeneratorParameter {
public:
	DCDContactGeneratorParameter(const DCDType& type, const DCDParameter& para) : ContactGeneratorParameter() {
		_dcd_type = type;
		_dcd_parameter = para.Clone();
	}
	DERIVED_DECLARE_CLONE(ContactGeneratorParameter)
	DECLARE_OVERWRITE_ACCESSIBLE_MEMBER(DCDType, DCDType, _dcd_type)
	DECLARE_OVERWRITE_ACCESSIBLE_POINTER_MEMBER(DCDParameter, DCDPara, _dcd_parameter)
};

class DCDContactGenerator : public ContactGenerator {
public:
	void Initialize(const ContactGeneratorParameter &para) override;
	void GetContact(const System &system, vector<ContactPoint> &contact_points) const override;

	virtual ~DCDContactGenerator() {
		delete _dcd;
	}
protected:
	DCD* _dcd;
};

#endif //FEM_DCDCONTACTGENERATOR_H
