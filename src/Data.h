#pragma once

#include <glad/glad.h>

struct Data {
    unsigned int VBO, VAO, EBO;
    GLsizei indexCount;
    GLenum primitiveType;
    GLenum indexType;
};