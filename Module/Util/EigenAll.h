//
// Created by hansljy on 2022/3/20.
//

#ifndef FEM_EIGENALL_H
#define FEM_EIGENALL_H

#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <Eigen/Geometry>
#include <Eigen/Core>

using Eigen::Matrix;
using Eigen::MatrixXd;
using Eigen::VectorXd;
using Eigen::Matrix3d;
using Eigen::Vector3d;
using Eigen::Matrix4d;
using Eigen::Vector4d;
using Eigen::MatrixX;
using Eigen::VectorX;
using Eigen::MatrixXi;

using Eigen::RowVector3i;

using Eigen::Dynamic;

typedef Eigen::Vector<double, 9> Vector9d;
typedef Eigen::Matrix<double, 9, 9> Matrix9d;
typedef Eigen::Vector<double, 12> Vector12d;
typedef Eigen::Matrix<double, 12, 12> Matrix12d;
typedef Eigen::Matrix<double, 12, 9> Matrix12x9d;
typedef Eigen::Vector<double, 1> Vector1d;
typedef Eigen::SparseMatrix<double> SparseMatrixXd;
typedef Eigen::Triplet<double> Tripletd;
typedef std::vector<Tripletd> COO;

using Eigen::AngleAxisd;

#endif //FEM_EIGENALL_H
