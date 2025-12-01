#include "Paddle.h"

Paddle::Paddle(glm::vec3 position, float r, float g, float b) {
    vertices = {
        // Position          // Color
         0.015f,  0.175f, 0.0f,  r, g, b, // Top Right
        -0.015f,  0.175f, 0.0f,  r, g, b, // Top Left
        -0.015f, -0.175f, 0.0f,  r, g, b, // Left Bottom       
         0.015f, -0.175f, 0.0f,  r, g, b  // Right Bottom
    };

    indices = {
        0u, 1u, 3u, // First Triangle
        1u, 2u, 3u  // Second Triangle
    };

    pos = position;
    vel = glm::vec3(0.0f);
}

void Paddle::setupData(GLenum shapeType) {
    data.indexCount = static_cast<GLsizei>(indices.size());
    data.primitiveType = shapeType;
    data.indexType = GL_UNSIGNED_INT;

    glGenVertexArrays(1, &data.VAO);
    glGenBuffers(1, &data.VBO);
    glGenBuffers(1, &data.EBO);

    glBindVertexArray(data.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, data.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Paddle::drawShape() {
    move();

    glBindVertexArray(data.VAO);

    glDrawElements(
        data.primitiveType,
        data.indexCount,
        data.indexType,
        nullptr
    );

    glBindVertexArray(0);
}

void Paddle::move() {
    pos += vel;

    glm::mat4 model(1.0f);
    model = glm::translate(model, pos);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
}

void Paddle::changeVelocity(glm::vec3 velocity) {
    vel = velocity;
}

void Paddle::initModelLoc(const Shader& shader) {
    modelLoc = glGetUniformLocation(shader.ID, "model");
}

void Paddle::bindVertexArray() {
    glBindVertexArray(data.VAO);
}

void Paddle::deleteArrays() {
    glDeleteVertexArrays(1, &data.VAO);
    glDeleteBuffers(1, &data.VBO);
    glDeleteBuffers(1, &data.EBO);
}

void Paddle::handleBorderCollisions() {
    if (pos.y + 0.175f > 1.0f) {
        pos.y = 1.0f - 0.175f;
    }

    if (pos.y - 0.175f < -1.0f) {
        pos.y = -1.0f + 0.175f;
    }
}
