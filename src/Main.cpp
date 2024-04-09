#include <raylib.h>

#include "GameScene.hpp"

const int screenWidth = 1600;
const int screenHeight = 900;

const int rows = 6;
const int cols = 7;


int main() {

	InitWindow(screenWidth, screenHeight, "Connect 4");

	InitAudioDevice();

	SetTargetFPS(60);

	Font font = LoadFont("resources/Rubik-ExtraBold.ttf");

	GameScene mainScene(screenWidth, screenHeight, cols, rows, GameMode::VsPlayer);
	mainScene.SetFont(font);

	while (!WindowShouldClose()) {
		BeginDrawing();

		ClearBackground(BLACK);

		mainScene.Render();

		EndDrawing();
	}

	UnloadFont(font);

	CloseAudioDevice();

	CloseWindow();


	return 0;
}