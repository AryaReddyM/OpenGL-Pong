#pragma once

#include <random>

class RandomNumGen {
public:
	static float GenerateFloat(float min, float max) {
		std::random_device rd;
		std::mt19937 gen;

		gen = std::mt19937(rd());

		std::uniform_real_distribution<float> dis(min, max);

		return dis(gen);
	}

	static int GenerateInt(int min, int max) {
		std::random_device rd;
		std::mt19937 gen;

		gen = std::mt19937(rd());

		std::uniform_int_distribution<> dis(min, max);

		return dis(gen);
	}
};