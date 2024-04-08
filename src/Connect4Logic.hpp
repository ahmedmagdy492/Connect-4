#pragma once

#include "Player.hpp"

#define COLS 7
#define ROWS 6

struct PlayState {
	bool someoneWon = false;
	bool success = false;
};


class Connect4Logic {
private:
	bool isPlayer1Turn = true;

	int matrix[ROWS][COLS];

	void InitMatrix() {
		for (int i = 0; i < ROWS; ++i) {
			for (int j = 0; j < COLS; ++j) {
				matrix[i][j] = 0;
			}
		}
	}

	void DeterminePlayerPosition() {
		isPlayer1Turn = GetRandomValue(0, 10) % 1;
	}

public:
	Connect4Logic() {
		ResetGameState();
	}

	void ResetGameState() {
		InitMatrix();
		DeterminePlayerPosition();
	}

	bool IsPlayer1Turn() {
		return isPlayer1Turn;
	}

	void SwitchPlayerTurn() {
		isPlayer1Turn = !isPlayer1Turn;
	}

	Vector2 GetLowestIndexInCol(Vector2 inputVec) {
		Vector2 matrixVec = { COLS, ROWS-1 };
		int underneethSlotsCount = matrixVec.y - inputVec.y;
		int i, c;

		for (int y = matrixVec.y; y >= 0; --y) {
			if (matrix[y][(int)inputVec.x] == 0) {
				Vector2 finalVec = { inputVec.x, y };
				return finalVec;
			}
		}

		return inputVec;
	}

	PlayState Play(Vector2 inputVec) {
		if (matrix[(int)inputVec.y][(int)inputVec.x] != 0) {
			return { false, false };
		}

		matrix[(int)inputVec.y][(int)inputVec.x] = IsPlayer1Turn() ? 1 : 2;

		// TODO: check if someone won or not

		return { false, true };
	}
};

