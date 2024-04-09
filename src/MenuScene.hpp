#pragma once

#include <iostream>

#include <raylib.h>

using namespace std;

class MenuScene {
private:
	Texture logo = LoadTexture("resources/logo.png");
	int screenWidth, screenHeight;
	Font font;

public:
	MenuScene(Font font, int screenWidth, int screenHeight) : screenWidth(screenWidth), screenHeight(screenHeight), font(font) {
	}

	~MenuScene() {
		UnloadTexture(logo);
	}

	int Render() {
		/*const char* logo = "Connect 4";
		Vector2 textureSize = MeasureTextEx(font, logo, 50, 0);
		Vector2 texturePos = { (screenWidth - textureSize.x) / 2, 100 };
		DrawTextEx(font, logo, texturePos, 50, 0, WHITE);*/
		Vector2 texturePos = { (screenWidth - logo.width)/2, -100};
		DrawTexture(logo, texturePos.x, texturePos.y, WHITE);

		DrawRectangle(0, 0, screenWidth, screenHeight, Color{0, 0, 0, 155});

		const char* winWord = "Vs Player";
		Vector2 winWordSize = MeasureTextEx(font, winWord, 40, 0);
		Vector2 winWordPos = { (screenWidth - winWordSize.x) / 2, (screenHeight - winWordSize.y) / 2};
		DrawTextEx(font, winWord, winWordPos, 40, 0, WHITE);

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			Vector2 mousePos = GetMousePosition();

			if ((mousePos.x <= screenWidth && mousePos.x > 0) && (mousePos.y >= 0 && mousePos.y <= screenHeight)) {
				
			}
		}

		const char* winWord2 = "Vs Com";
		Vector2 winWord2Size = MeasureTextEx(font, winWord2, 40, 0);
		Vector2 winWord2Pos = { (screenWidth - winWord2Size.x) / 2, winWordPos.y + winWordSize.y + 20 };
		DrawTextEx(font, winWord2, winWord2Pos, 40, 0, WHITE);

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			Vector2 mousePos = GetMousePosition();

			if ((mousePos.x <= screenWidth && mousePos.x > 0) && (mousePos.y >= 0 && mousePos.y <= screenHeight)) {
				return 2;
			}
		}

		const char* winWord3 = "Exit";
		Vector2 winWord3Size = MeasureTextEx(font, winWord3, 40, 0);
		Vector2 winWord3Pos = { (screenWidth - winWord3Size.x) / 2, winWord2Pos.y + winWord2Size.y + 20 };
		DrawTextEx(font, winWord3, winWord3Pos, 40, 0, WHITE);

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			Vector2 mousePos = GetMousePosition();

			if ((mousePos.x <= screenWidth && mousePos.x > 0) && (mousePos.y >= 0 && mousePos.y <= screenHeight)) {
				return 0;
			}
		}

		return 1;
	}
};