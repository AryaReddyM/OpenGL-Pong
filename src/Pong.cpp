#include "Pong.h"

int main() {
	Pong pong = Pong();

	return 0;
}

Pong::Pong() : rightPaddle(glm::vec3(0.95f, 0.0f, 0.0f), 0.0f, 0.0f, 1.0f), 
	leftPaddle(glm::vec3(-0.95f, 0.0f, 0.0f), 1.0f, 0.0f, 0.0f), 
	ball(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)) {
	//////////////// Initializing ALL OpengGL Stuff ////////////////

	// Initializing GLFW and Configuring
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_MAXIMIZED, GL_TRUE);

	window = glfwCreateWindow(screenWidth, screenHeight, "Pong", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);
	// register static member function as framebuffer callback
	
	glfwSetFramebufferSizeCallback(window, Pong::framebuffer_size_callback);

	// Load all OpenGL function pointers with GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	//////////////// Build and Compile Shader Programs ////////////////

	Shader shader = Shader("C:/C++ Projects/OpenGL/Pong/src/shader.vs", "C:/C++ Projects/OpenGL/Pong/src/shader.fs");

	//////////////// Initalizing objects ////////////////

	rightPaddle.initModelLoc(shader);
	leftPaddle.initModelLoc(shader);
	ball.initModelLoc(shader);

	rightPaddle.setupData(GL_TRIANGLES);
	leftPaddle.setupData(GL_TRIANGLES);
	ball.setupData(GL_TRIANGLE_FAN);

	TextRenderer leftScore(screenWidth, screenHeight, "C:/C++ Projects/OpenGL/Pong/assets/fonts/OpenSans-Regular.ttf");
	TextRenderer rightScore(screenWidth, screenHeight, "C:/C++ Projects/OpenGL/Pong/assets/fonts/OpenSans-Regular.ttf");

	bool leftHitBorder = false;
	bool rightHitBorder = false;

	//////////////// Render Loop ////////////////
		float lastActionTime = float(glfwGetTime());

		while (!glfwWindowShouldClose(window)) {
			// Handling all input
			processInput();

			// Set Color
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Wireframe Mode
			//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL). // Regular Mode

			// Activate shader
			shader.use();

			rightPaddle.bindVertexArray();
			leftPaddle.bindVertexArray();
			ball.bindVertexArray();

			// Handle Collision
			rightPaddle.handleBorderCollisions();
			leftPaddle.handleBorderCollisions();
			ball.handleWallCollisions(rightScoreNum, leftScoreNum);

			ball.handlePaddleCollisions(rightPaddle, rightScoreNum);
			ball.handlePaddleCollisions(leftPaddle, leftScoreNum);
		
			// Draw shape
			rightPaddle.drawShape();
			leftPaddle.drawShape();
			ball.drawShape(shader);

			// Draw Score
			rightScore.RenderText("Score: " + std::to_string(rightScoreNum), float(screenWidth) - 225.0f, float(screenHeight - 80), 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
			leftScore.RenderText("Score: " + std::to_string(leftScoreNum), 35.0f, float(screenHeight - 80), 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));

			// Check for Ball Velocity Delay
			if (!initBallVel) {
				float currentTime = float(glfwGetTime());
				if (currentTime - lastActionTime >= ballVelDelay) {
					ball.initVelocity();

					initBallVel = true;

					lastActionTime = currentTime;
				}
			}

			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		rightPaddle.deleteArrays();
		leftPaddle.deleteArrays();
		ball.deleteArrays();

		glfwTerminate();
	}

void Pong::processInput() {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		rightPaddle.changeVelocity(glm::vec3(0.0f, 0.002f, 0.0f));
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		rightPaddle.changeVelocity(glm::vec3(0.0f, -0.002f, 0.0f));
	}
	else {
		rightPaddle.changeVelocity(glm::vec3(0.0f));
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		leftPaddle.changeVelocity(glm::vec3(0.0f, 0.002f, 0.0f));
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		leftPaddle.changeVelocity(glm::vec3(0.0f, -0.002f, 0.0f));
	}
	else {
		leftPaddle.changeVelocity(glm::vec3(0.0f));
	}
}


void Pong::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
}