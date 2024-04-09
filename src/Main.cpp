#include <raylib.h>

#include "GameScene.hpp"
#include "MenuScene.hpp"

const int screenWidth = 1600;
const int screenHeight = 900;

const int rows = 6;
const int cols = 7;


int main() {

	InitWindow(screenWidth, screenHeight, "Connect 4");

	Font font = LoadFont("resources/Rubik-ExtraBold.ttf");

	BeginDrawing();
	ClearBackground(BLACK);
	DrawRectangle(0, 0, screenWidth, screenHeight, Color{ 0, 0, 0, 180 });
	Vector2 loading_txt_size = MeasureTextEx(font, "Ahmed Magdy", 40, 0);
	DrawTextEx(font, "Ahmed Magdy", { (screenWidth - loading_txt_size.x) / 2, (screenHeight - loading_txt_size.y) / 2 }, 40, 0, WHITE);
	EndDrawing();

	SetExitKey(0);


	InitAudioDevice();

	SetTargetFPS(60);

	Sound bgSound = LoadSound("resources/bg.mp3");

	GameScene mainScene(screenWidth, screenHeight, cols, rows, GameMode::VsPlayer);
	mainScene.SetFont(font);

	MenuScene menuScene(font, screenWidth, screenHeight);

	int resCode = 1;

	while (!WindowShouldClose()) {
		BeginDrawing();

		ClearBackground(BLACK);

		if (!IsSoundPlaying(bgSound)) {
			PlaySound(bgSound);
		}

		if (resCode == 0) {
			break;
		}
		else if (resCode == 2) {
			resCode = mainScene.Render();
		}
		else if (resCode == 1) {
			resCode = menuScene.Render();
		}

		EndDrawing();
	}

	UnloadFont(font);

	UnloadSound(bgSound);

	CloseAudioDevice();

	CloseWindow();

	return 0;
}