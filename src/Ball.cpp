#include "Ball.h"
#include <iostream>

Ball::Ball(glm::vec3 position, glm::vec3 color) {
    vertices.push_back({ position, color });

    for (int i = 0; i <= segments; i++) {
        float angle = 2.0f * glm::pi<float>() * static_cast<float>(i) / static_cast<float>(segments);

        float x = position.x + radius * cos(angle);
        float y = position.y + radius * sin(angle);
        float z = position.z;

        vertices.push_back({ {x, y, z}, color });
    }

    for (int i = 0; i < segments; i++) {
        unsigned int p1_index = i + 1;
        unsigned int p2_index = i + 2;

        if (int(p2_index) > segments) {
            p2_index = 1;
        }

        indices.push_back(0);
        indices.push_back(p1_index);
        indices.push_back(p2_index);
    }
}


void Ball::setupData(GLenum shapeType) {
    data.indexCount = static_cast<unsigned int>(indices.size());
    data.primitiveType = shapeType;
    data.indexType = GL_UNSIGNED_INT;

    glGenVertexArrays(1, &data.VAO);
    glGenBuffers(1, &data.VBO);
    glGenBuffers(1, &data.EBO);

    glBindVertexArray(data.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, data.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    firstFrame = glfwGetTime();
}

void Ball::drawShape(Shader& shader) {
    move(shader);

    glBindVertexArray(data.VAO);
    glDrawElements(data.primitiveType, data.indexCount, data.indexType, nullptr);
    glBindVertexArray(0);
}

void Ball::move(Shader& shader) {
    pos += vel;

    glm::mat4 model(1.0f);

    model = glm::translate(model, pos);

    float targetAspectRatio = 1920.0f / 1080.0f;
    model = glm::scale(model, glm::vec3(1.0f, targetAspectRatio, 1.0f));


    shader.use();

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
}


void Ball::changeVelocity(glm::vec3 velocity) {
    vel = velocity;
}

void Ball::initModelLoc(const Shader& shader) {
    modelLoc = glGetUniformLocation(shader.ID, "model");
}

void Ball::bindVertexArray() {
    glBindVertexArray(data.VAO);
}

void Ball::deleteArrays() {
    glDeleteVertexArrays(1, &data.VAO);
    glDeleteBuffers(1, &data.VBO);
    glDeleteBuffers(1, &data.EBO);
}

void Ball::initVelocity() {
    int num = RandomNumGen::GenerateInt(1, 2);

    if (num == 1) {
        vel.x = -speed;
    }
    else {
        vel.x = speed;
    }

    vel.y = RandomNumGen::GenerateFloat(-speed, speed);
}

void Ball::handleWallCollisions(int& rightScore, int& leftScore) {
    if (pos.x + radius > 1.0f) {
        vel.x *= -1.0f;
        vel.y = RandomNumGen::GenerateFloat(-speed, speed);
        rightScore = 0;
    }
    if (pos.x - radius < -1.0f ) {
        vel.x *= -1.0f;
        vel.y = RandomNumGen::GenerateFloat(-speed, speed);
        leftScore = 0;
    }

    if (pos.y + radius > 1.0f || pos.y - radius < -1.0f) {
        vel.y *= -1.0f;
    }
}

void Ball::handlePaddleCollisions(Paddle paddle, int& score) {
    const Paddle& p = paddle;

    glm::vec3 tempPos = pos;
    glm::vec3 step = vel / float(subSteps);

    for (int i = 1; i <= subSteps; ++i) {
        tempPos += step;

        glm::vec3 halfSize = p.size * 0.5f;
        glm::vec2 rectMin = glm::vec2(p.pos.x - halfSize.x, p.pos.y - halfSize.y);
        glm::vec2 rectMax = glm::vec2(p.pos.x + halfSize.x, p.pos.y + halfSize.y);

        glm::vec2 circleCenter = glm::vec2(tempPos.x, tempPos.y);
        float closestX = glm::clamp(circleCenter.x, rectMin.x, rectMax.x);
        float closestY = glm::clamp(circleCenter.y, rectMin.y, rectMax.y);
        glm::vec2 closestPoint(closestX, closestY);

        glm::vec2 diff = circleCenter - closestPoint;
        float distSq = glm::dot(diff, diff);

        if (distSq <= radius * radius) {
            float currentFrame = glfwGetTime();

            if (currentFrame - firstFrame >= paddleCollisionDelay) {
                if (vel.x < 0.0f) {
                    std::cout << "Red: " << i << std::endl;
                }
                else {
                    std::cout << "Blue: " << i << std::endl;
                }

                vel.x *= -1.0f;

                vel.y = RandomNumGen::GenerateFloat(-speed, speed);
                score++;

                firstFrame = currentFrame;
            }

            break;
        }
    }
}
