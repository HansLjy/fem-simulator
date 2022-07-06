#include "MeshObject.hpp"
#include <cstdlib>
#include "spdlog/spdlog.h"
#include <iostream>

MeshObject::MeshObject() {
    glGenVertexArrays(1, &_VAO);
}

void MeshObject::SetMesh(const MatrixXd &vertices, const MatrixXi &topos) {
    // TODO: make this more efficient
    if (_vertex_array_data.rows() != topos.size() || _vertex_array_data.cols() != 6) {
        _vertex_array_data.resize(topos.size(), 6);
    }

    int num_triangles = topos.rows();
    for (int i = 0, cnt_rows = 0; i < num_triangles; i++, cnt_rows += 3) {
        Eigen::RowVector3i indices = topos.row(i);
        Eigen::RowVector3f vertex[3];
        for (int j = 0; j < 3; j++) {
            vertex[j] = vertices.row(indices[j]).cast<float>();
        }
        Eigen::RowVector3f normal = ((vertex[1] - vertex[0]).cross(vertex[2] - vertex[0])).cast<float>();

        for (int j = 0; j < 3; j++) {
            _vertex_array_data.block<1, 3>(cnt_rows + j, 0) = vertex[j];
            _vertex_array_data.block<1, 3>(cnt_rows + j, 3) = normal;
        }
    }

    BindVertexArray();
}

void MeshObject::BindVertexArray() {
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindVertexArray(_VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, _vertex_array_data.size() * sizeof(float), _vertex_array_data.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glDeleteBuffers(1, &VBO);
}

void MeshObject::Bind() {
    glBindVertexArray(_VAO);
}

void MeshObject::Draw() {
    glDrawArrays(GL_TRIANGLES, 0, _vertex_array_data.rows());
}

MeshObject::~MeshObject() {
    if (_VAO != 0) {
        glDeleteVertexArrays(1, &_VAO);
    }
}

MeshObject::MeshObject(const MeshObject& mesh) {
    _vertex_array_data = mesh._vertex_array_data;
    glGenVertexArrays(1, &_VAO);
    BindVertexArray();
}