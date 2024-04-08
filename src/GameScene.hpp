#pragma once

#include <iostream>
#include <vector>

#include <raylib.h>
#include "Slot.hpp"
#include "GameMode.hpp"
#include "Connect4Logic.hpp"

using namespace std;

class GameScene {
private:
	const int fromTopOffset = 150;

	int screenWidth, screenHeight;
	int cols, rows;

	GameMode currentGameMode;
	Slot slots[ROWS][COLS];
	Font font;

	Connect4Logic connect4Logic;

	Player* players[2];

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

	}

	void UnInitalize() {
		delete players[0];
		delete players[1];
	}

	void Draw() {
		DrawRectangle(0, 0, screenWidth, 50, BLUE);

		DrawRectangle(0, fromTopOffset, screenWidth, screenHeight, BLUE);

		bool isPlayer1Turn = connect4Logic.IsPlayer1Turn();
		Player* curPlayer = players[isPlayer1Turn];

		DrawTextEx(font, isPlayer1Turn ? "Player 1" : "Player 2", { 10, 10 }, 25, 0, WHITE);
		Vector2 txtSize = MeasureTextEx(font, "Player 2", 25, 0);
		DrawRectangle(txtSize.x + 20, 12, 30, 20, curPlayer->GetColor());

		for (int i = 0; i < ROWS; ++i) {
			for (int j = 0; j < COLS; ++j) {
				Slot slot = slots[i][j];
				Vector2 size = slot.GetSize();
				Vector2 pos = slot.GetPosition();
				DrawRectangle(pos.x, pos.y, size.x, size.y, slot.GetColor());
			}
		}
	}

	void HandleUserInput() {
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			Vector2 clickedObjectIndex = GetClickedSlotIndex();

			if (clickedObjectIndex.x != -1 && clickedObjectIndex.y != -1) {
				//Vector2 mappedIndex = MapMousePosToMatrixIndex(clickedObjectIndex);
				Vector2 finalIndex = connect4Logic.GetLowestIndexInCol(clickedObjectIndex);

				PlayState playState = connect4Logic.Play(finalIndex);

				if (playState.success) {
					Player* currentPlayer = players[connect4Logic.IsPlayer1Turn()];
					slots[(int)finalIndex.y][(int)finalIndex.x].SetColor(currentPlayer->GetColor());

					if (playState.someoneWon) {
						// TODO: move to wining scene
					}
					else {
						connect4Logic.SwitchPlayerTurn();

						if (currentGameMode == GameMode::VsComputer && !connect4Logic.IsPlayer1Turn()) {
							// TODO: implement computer game play here
						}
					}
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

	void Render() {
		Draw();
		HandleUserInput();
	}
};