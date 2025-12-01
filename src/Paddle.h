#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Data.h"

class Paddle {
public:
    Paddle(glm::vec3 position, float r, float g, float b);

    void setupData(GLenum shapeType);
    void drawShape();
    void move();
    void changeVelocity(glm::vec3 velocity);
    void initModelLoc(const Shader& shader);
    void bindVertexArray();
    void deleteArrays();
    void handleBorderCollisions();

    Data data;

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    glm::vec3 pos;
    glm::vec3 vel;

    glm::vec3 size = glm::vec3(0.03f, 0.325f, 0.0f);

    int modelLoc;
};