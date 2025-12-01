#pragma once

#include "Ball.h"

class Pong {
public:
    Pong();

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void processInput();

    // Screen sizes
    const unsigned int screenWidth = 1920;
    const unsigned int screenHeight = 1080;
    const float targetAspectRatio = float(1920) / float(1080);
    GLFWwindow* window;

    unsigned int projectionLoc;

    // Paddles
    Paddle rightPaddle;
	Paddle leftPaddle;

    // Ball
    Ball ball;

    int leftScoreNum = 0;
    int rightScoreNum = 0;

    bool initBallVel = false;
    float ballVelDelay = 2.0f;

    const int subSteps = 5;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
};