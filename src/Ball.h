#pragma once

#include <vector>
#include "Data.h"
#include "GenerateRandomNum.h"
#include <cmath>
#include <iostream>
#include "Paddle.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
};

class Ball {
public:
    Ball(glm::vec3 position, glm::vec3 color);
    void setupData(GLenum shapeType);
    void drawShape(Shader& shader);
    void move(Shader& shader);
    void changeVelocity(glm::vec3 velocity);
    void initModelLoc(const Shader& shader);
    void bindVertexArray();
    void deleteArrays();
    void initVelocity();
    void handleWallCollisions(int& rightScore, int& leftScore);
    void handlePaddleCollisions(Paddle paddle, int& score);

    float radius = 0.025f;
    glm::vec3 pos;
    glm::vec3 vel;
    glm::vec3 stepVel;
    int segments = 100;
    std::vector<Vertex> vertices;
    std::vector<int> indices;
    int modelLoc;

    float speed = 0.00075f;

    glm::vec3 size = glm::vec3(radius * 2.0f, radius * 2.0f, 0.0f);

    Data data;

    const int subSteps = 5;

    float firstFrame;
    float paddleCollisionDelay = 0.5f;
};