//
// Created by hansljy on 2022/4/9.
//

#ifndef FEM_POLYHEDRALCONE_H
#define FEM_POLYHEDRALCONE_H

#include "ContactGenerator.h"

class PolyhedralCone : public ContactGenerator {
public:
	void
	GetContact(const System &system, SparseMatrixXd &Jn, SparseMatrixXd &Jt,
			   VectorXd &mu) const override;

};

#endif //FEM_POLYHEDRALCONE_H
