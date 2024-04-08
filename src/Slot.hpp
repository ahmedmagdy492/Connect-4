#pragma once

#include <raylib.h>

class Slot {
private:
	Color color;
	Vector2 size;
	Vector2 position;

public:
	Slot() {}

	Slot(Color color, Vector2 size, Vector2 position) : color(color), size(size), position(position) {
	}

	Color GetColor() {
		return color;
	}

	const Vector2& GetSize() {
		return size;
	}

	const Vector2& GetPosition() {
		return position;
	}

	void SetColor(Color color) {
		this->color = color;
	}
};