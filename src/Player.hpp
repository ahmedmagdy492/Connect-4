#pragma once

#include <raylib.h>

class Player {
private:
	Color color;
	unsigned char value;

public:
	Player(Color color, unsigned char value) : color(color), value(value) {
	}

	Color GetColor() {
		return color;
	}

	unsigned char GetValue() {
		return value;
	}
};