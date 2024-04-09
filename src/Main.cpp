#include <raylib.h>

#include "GameScene.hpp"
#include "MenuScene.hpp"

const int screenWidth = 1600;
const int screenHeight = 900;

const int rows = 6;
const int cols = 7;


int main() {

	InitWindow(screenWidth, screenHeight, "Connect 4");

	InitAudioDevice();

	SetTargetFPS(60);

	SetExitKey(0);

	Font font = LoadFont("resources/Rubik-ExtraBold.ttf");

	GameScene mainScene(screenWidth, screenHeight, cols, rows, GameMode::VsPlayer);
	mainScene.SetFont(font);

	MenuScene menuScene(font, screenWidth, screenHeight);

	int resCode = 1;

	while (!WindowShouldClose()) {
		BeginDrawing();

		ClearBackground(BLACK);

		if (!resCode) {
			break;
		}
		else if (resCode == 2) {
			mainScene.Render();
		}
		else if (resCode == 1) {
			resCode = menuScene.Render();
		}

		EndDrawing();
	}

	UnloadFont(font);

	CloseAudioDevice();

	CloseWindow();

	return 0;
}