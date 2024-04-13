#pragma once

#include <iostream>
#include <vector>

#include <raylib.h>
#include "Slot.hpp"
#include "GameMode.hpp"
#include "Connect4Logic.hpp"

using namespace std;

enum ScreenMode {
	PlayingScreen,
	WiningScreen,
	PauseScreen,
	ExitScreen,
	DrawScreen
};

class GameScene {
private:
	const int fromTopOffset = 150;

	int screenWidth, screenHeight;
	int cols, rows;
	int wonPlayer = -1;

	ScreenMode currentScreenMode;

	GameMode currentGameMode;
	Slot slots[ROWS][COLS];
	Font font;

	Connect4Logic connect4Logic;

	Player* players[2];

	Sound sound = LoadSound("resources/sound.ogg");
	Sound winSound = LoadSound("resources/win.wav");

	void InitValues() {
		int squareWidth = screenWidth / cols - ((cols - 5.9) * 10);
		int squareHeight = screenHeight / rows - ((rows - 2.3) * 10);

		for (int j = 0; j < rows; ++j) {
			for (int i = 0; i < cols; ++i) {
				Vector2 size = { squareWidth, squareHeight };
				Vector2 position = { 10 * (i + 1) + i * squareWidth, fromTopOffset + 10 * (j + 1) + j * squareHeight };
				Slot slot(BLACK, size, position);
				slots[j][i] = slot;
			}
		}

		// creating players
		players[0] = new Player(RED, 1);
		players[1] = new Player(YELLOW, 2);

		currentScreenMode = ScreenMode::PlayingScreen;
	}

	void UnInitalize() {
		delete players[0];
		delete players[1];
	}

	void Draw() {
		DrawRectangle(0, 0, screenWidth, 50, BLUE);

		DrawRectangle(0, fromTopOffset, screenWidth, screenHeight, BLUE);

		const char* pauseMenuWord = "Pause";
		Vector2 pauseMenuWordSize = MeasureTextEx(font, pauseMenuWord, 25, 0);
		Vector2 pauseMenuWordPos = { screenWidth - pauseMenuWordSize.x - 10, 10 };
		DrawTextEx(font, pauseMenuWord, pauseMenuWordPos, 25, 0, WHITE);

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			Vector2 mousePos = GetMousePosition();

			if ((mousePos.x >= pauseMenuWordPos.x && mousePos.x <= (pauseMenuWordPos.x + pauseMenuWordSize.x)) && (mousePos.y >= 10 && mousePos.y <= (pauseMenuWordPos.y + pauseMenuWordSize.y)) ) {
				currentScreenMode = ScreenMode::PauseScreen;
			}
		}

		const char* exitWord = "Reset Game";
		Vector2 exitWordSize = MeasureTextEx(font, exitWord, 25, 0);
		Vector2 exitWordPos = { pauseMenuWordPos.x - exitWordSize.x - 40, 10 };
		DrawTextEx(font, exitWord, exitWordPos, 25, 0, WHITE);

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			Vector2 mousePos = GetMousePosition();

			if ((mousePos.x >= exitWordPos.x && mousePos.x <= (exitWordPos.x + exitWordSize.x)) && (mousePos.y >= 10 && mousePos.y <= (exitWordPos.y + exitWordSize.y))) {
				currentScreenMode = ScreenMode::ExitScreen;
				UnInitalize();
				InitValues();
				connect4Logic.ResetGameState();
			}
		}

		bool isPlayer1Turn = connect4Logic.IsPlayer1Turn();
		Player* curPlayer = players[isPlayer1Turn];
		if (currentGameMode == GameMode::VsComputer) {
			DrawTextEx(font, isPlayer1Turn ? "Player 1" : "Computer", { 10, 10 }, 25, 0, WHITE);
			Vector2 txtSize = MeasureTextEx(font, "Computer", 25, 0);
			DrawRectangle(txtSize.x + 20, 12, 30, 20, curPlayer->GetColor());
		}
		else {
			DrawTextEx(font, isPlayer1Turn ? "Player 1" : "Player 2", { 10, 10 }, 25, 0, WHITE);
			Vector2 txtSize = MeasureTextEx(font, "Player 2", 25, 0);
			DrawRectangle(txtSize.x + 20, 12, 30, 20, curPlayer->GetColor());
		}

		for (int i = 0; i < ROWS; ++i) {
			for (int j = 0; j < COLS; ++j) {
				Slot slot = slots[i][j];
				Vector2 size = slot.GetSize();
				Vector2 pos = slot.GetPosition();
				DrawRectangleRounded({ pos.x, pos.y, size.x, size.y }, 150, 10, slot.GetColor());
			}
		}
	}

	void HandleUserInput() {
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			Vector2 clickedObjectIndex = GetClickedSlotIndex();

			if (clickedObjectIndex.x != -1 && clickedObjectIndex.y != -1) {
				Vector2 finalIndex = connect4Logic.GetLowestIndexInCol(clickedObjectIndex);

				PlayState playState = connect4Logic.Play(finalIndex);

				if (playState.success) {
					PlaySound(sound);
					Player* currentPlayer = players[connect4Logic.IsPlayer1Turn()];
					slots[(int)finalIndex.y][(int)finalIndex.x].SetColor(currentPlayer->GetColor());

					if (playState.someoneWon) {
						// move to wining scene
						wonPlayer = playState.whoWon;
						currentScreenMode = ScreenMode::WiningScreen;
						PlaySound(winSound);
					}
					else if (playState.isItADraw) {
						currentScreenMode = ScreenMode::DrawScreen;
					}
					else {
						connect4Logic.SwitchPlayerTurn();

						if (currentGameMode == GameMode::VsComputer) {
							Vector2 bestPosition = connect4Logic.GetBestPosition();
							playState = connect4Logic.Play(bestPosition);

							if (playState.success) {
								PlaySound(sound);
								Player* currentPlayer = players[connect4Logic.IsPlayer1Turn()];
								slots[(int)bestPosition.y][(int)bestPosition.x].SetColor(currentPlayer->GetColor());

								if (playState.someoneWon) {
									// move to wining scene
									wonPlayer = playState.whoWon;
									currentScreenMode = ScreenMode::WiningScreen;
									PlaySound(winSound);
								}
								connect4Logic.SwitchPlayerTurn();
							}
						}
					}
				}
			}
		}
	}

	void DrawWiningScreen() {
		Vector2 winWordSize;
		Vector2 winWordPos;

		if (currentGameMode == GameMode::VsPlayer) {
			const char* winWord = wonPlayer == 1 ? "Player1 Won Click Here To Retry" : "Player2 Won Click Here To Retry";
			winWordSize = MeasureTextEx(font, winWord, 30, 0);
			winWordPos = { (screenWidth - winWordSize.x) / 2, 80 };
			DrawTextEx(font, winWord, winWordPos, 30, 0, WHITE);
		}
		else {
			const char* winWord = wonPlayer == 1 ? "Player1 Won Click Here To Retry" : "Computer Won Click Here To Retry";
			winWordSize = MeasureTextEx(font, winWord, 30, 0);
			winWordPos = { (screenWidth - winWordSize.x) / 2, 80 };
			DrawTextEx(font, winWord, winWordPos, 30, 0, WHITE);
		}

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			Vector2 mousePos = GetMousePosition();

			if ((mousePos.x <= screenWidth && mousePos.x > 0) && (mousePos.y >= winWordPos.y && mousePos.y < (winWordPos.y + winWordSize.y))) {
				UnInitalize();
				InitValues();
				connect4Logic.ResetGameState();
			}
		}
	}

	void DrawDrawScreen() {
		const char* winWord = "Draw No One Won, Click Here To Retry";
		Vector2 winWordSize = MeasureTextEx(font, winWord, 30, 0);
		Vector2 winWordPos = { (screenWidth - winWordSize.x) / 2, 80 };
		DrawTextEx(font, winWord, winWordPos, 30, 0, WHITE);

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			Vector2 mousePos = GetMousePosition();

			if ((mousePos.x <= screenWidth && mousePos.x > 0) && (mousePos.y >= winWordPos.y && mousePos.y < (winWordPos.y + winWordSize.y))) {
				UnInitalize();
				InitValues();
				connect4Logic.ResetGameState();
			}
		}
	}

	void DrawPauseMenu() {
		const char* winWord = "Click On Any Cell to Play in That Column. Click AnyWhere to Resume";
		Vector2 winWordSize = MeasureTextEx(font, winWord, 28, 0);
		Vector2 winWordPos = { (screenWidth - winWordSize.x) / 2, (screenHeight - winWordSize.y) / 2 };
		DrawTextEx(font, winWord, winWordPos, 28, 0, YELLOW);

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			Vector2 mousePos = GetMousePosition();

			if ((mousePos.x <= screenWidth && mousePos.x > 0) && (mousePos.y >= 0 && mousePos.y <= screenHeight)) {
				if (wonPlayer == -1) {
					currentScreenMode = ScreenMode::PlayingScreen;
				}
				else {
					currentScreenMode = ScreenMode::WiningScreen;
				}
			}
		}
	}

public:
	GameScene(int screenWidth, int screenHeight, int cols, int rows, GameMode gameMode) : screenWidth(screenWidth), screenHeight(screenHeight), cols(cols), rows(rows), currentGameMode(gameMode) {

		InitValues();
	}

	~GameScene() {
		UnInitalize();
		UnloadSound(sound);
		UnloadSound(winSound);
	}

	void SetGameMode(GameMode gameMode) {
		currentGameMode = gameMode;
	}

	void SetFont(Font font) {
		this->font = font;
	}

	Vector2 GetClickedSlotIndex() {
		Vector2 mousePos = GetMousePosition();

		for (int i = 0; i < ROWS; ++i) {
			for (int j = 0; j < COLS; ++j) {
				Vector2 position = slots[i][j].GetPosition();
				Vector2 size = slots[i][j].GetSize();
				if ((mousePos.x > position.x && mousePos.x < (position.x + size.x)) && (mousePos.y > position.y && mousePos.y < (position.y + size.y))) {
					return { (float)j, (float)i };
				}
			}
		}

		return { -1, -1 };
	}

	Vector2 MapMousePosToMatrixIndex(int slotIndex) {
		Vector2 finalIndex;
		finalIndex.y = slotIndex / cols;
		finalIndex.x = slotIndex % cols;
		return finalIndex;
	}

	int Render() {
		switch (currentScreenMode) {
		case ScreenMode::PlayingScreen:
			Draw();
			HandleUserInput();
			break;
		case ScreenMode::WiningScreen:
			Draw();
			DrawWiningScreen();
			break;
		case ScreenMode::PauseScreen:
			DrawPauseMenu();
			break;
		case ScreenMode::ExitScreen:
			return 1;
			break;
		case ScreenMode::DrawScreen:
			Draw();
			DrawDrawScreen();
			break;
		}

		return 2;
	}
};