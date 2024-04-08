#include <raylib.h>

#include "GameScene.hpp"

const int screenWidth = 800;
const int screenHeight = 600;

const int rows = 6;
const int cols = 7;


int main() {

	InitWindow(screenWidth, screenHeight, "Connect 4");

	SetTargetFPS(60);

	GameScene mainScene(screenWidth, screenHeight, cols, rows, GameMode::VsPlayer);

	while (!WindowShouldClose()) {
		BeginDrawing();

		ClearBackground(BLACK);

		mainScene.Render();

		EndDrawing();
	}

	CloseWindow();

	return 0;
}