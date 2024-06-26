#pragma once

#include "Player.hpp"

#include <iostream>

using namespace std;

#define COLS 7
#define ROWS 6

struct PlayState {
	bool someoneWon = false;
	bool success = false;
	int whoWon = -1;
	bool isItADraw = false;
};


class Connect4Logic {
private:
	unsigned char currentPlayer; // will have either the value 1 or 2
	Vector2 winingPositions[COLS];

	int matrix[ROWS][COLS];

	void InitMatrix() {
		for (int i = 0; i < ROWS; ++i) {
			for (int j = 0; j < COLS; ++j) {
				matrix[i][j] = 0;
			}
		}
	}

	void DeterminePlayerTurn() {
		currentPlayer = 1;
	}

    /// <summary>
    /// it's missing if a player has played in a middle position so we need to check in both directions at once
    /// </summary>
    /// <param name="inputVec"></param>
    /// <param name="currentPlayer"></param>
    /// <returns></returns>
    int CheckIfSomeoneWon(Vector2 inputVec, int currentPlayer) {
		Vector2 matrixBoundary = { COLS-1, ROWS-1 };
		int counter = 0;

		// left line
		for (int i = inputVec.x; i >= 0; --i) {
			if (matrix[(int)inputVec.y][i] == currentPlayer) {
				winingPositions[counter] = { (float)i, inputVec.y };
				++counter;
			}
			else {
				break;
			}
		}

		if (counter == 4) {
			cout << "Player " << currentPlayer << " with left line" << endl;
			return currentPlayer;
		}
		else {
			counter = 0;
		}

		for (int i = inputVec.x; i < COLS; ++i) {
			if (matrix[(int)inputVec.y][i] == currentPlayer) {
				winingPositions[counter] = { (float)i, inputVec.y };
				++counter;
			}
			else {
				break;
			}
		}

		if (counter == 4) {
			cout << "Player " << currentPlayer << " with right line" << endl;
			return currentPlayer;
		}
		else {
			counter = 0;
		}

		for (int i = inputVec.y; i < ROWS; ++i) {
			if (matrix[i][(int)inputVec.x] == currentPlayer) {
				winingPositions[counter] = { inputVec.x, (float)i };
				++counter;
			}
			else {
				break;
			}
		}

		if (counter == 4) {
			cout << "Player " << currentPlayer << " with up" << endl;
			return currentPlayer;
		}
		else {
			counter = 0;
		}

		for (int i = inputVec.x, j = inputVec.y; i < COLS && j < ROWS; ++i, ++j) {
			if (matrix[j][i] == currentPlayer) {
				winingPositions[counter] = { (float)i, (float)j };
				++counter;
			}
			else {
				break;
			}
		}

		if (counter == 4) {
			cout << "Player " << currentPlayer << " with left diagnaol" << endl;
			return currentPlayer;
		}
		else {
			counter = 0;
		}

		for (int i = inputVec.x, j = inputVec.y; i >= 0 && j < ROWS; --i, ++j) {
			if (matrix[j][i] == currentPlayer) {
				winingPositions[counter] = {(float)i, (float)j};
				++counter;
			}
			else {
				break;
			}
		}

		if (counter == 4) {
			cout << "Player " << currentPlayer << " with right diagnaol" << endl;
			return currentPlayer;
		}
		else {
			counter = 0;
		}

		// checking row by row whether someone won or not
		for (int x = 0; x < ROWS; ++x) {
			for (int i = 0; i < COLS; ++i) {
				for (int j = i, c = 0; c < 4 && j < COLS; ++c, ++j) {
					if (matrix[x][j] != currentPlayer) {
						counter = 0;
						break;
					}
					else {
						winingPositions[counter] = { (float)j, (float)x};
						++counter;
					}
				}

				if (counter >= 4) {
					cout << "Player " << currentPlayer << " with overall check" << endl;
					return currentPlayer;
				}
				else {
					counter = 0;
				}
			}
		}

		// checking left diagnols
		for (int x = 0; x < ROWS; ++x) {
			int b = x;
			for (int i = 0; i < COLS; ++i) {
				int c = 0;
				b = x;
				for (int j = i; c < 4 && j < COLS && b < ROWS; ++j, ++b, ++c) {
					if (matrix[b][j] != currentPlayer) {
						counter = 0;
						break;
					}
					else {
						winingPositions[counter] = { (float)j, (float)b };
						++counter;
					}
				}
				if (counter >= 4) {
					cout << "Player " << currentPlayer << " with left diagnol check 2" << endl;
					return currentPlayer;
				}
				else {
					counter = 0;
				}
			}
		}

		// checking right diagnols
		for (int x = 0; x < ROWS; ++x) {
			int b = x;
			for (int i = COLS - 1; i >= 0; --i) {
				int c = 0;
				b = x;
				for (int j = i; c < 4 && j >= 0 && b < ROWS; --j, ++b, ++c) {
					if (matrix[b][j] != currentPlayer) {
						counter = 0;
						break;
					}
					else {
						winingPositions[counter] = { (float)j, (float)b };
						++counter;
					}
				}
				if (counter >= 4) {
					cout << "Player " << currentPlayer << " with right diagnol check 2" << endl;
					return currentPlayer;
				}
				else {
					counter = 0;
				}
			}
		}

		if (IsItADraw()) {
			// a draw
			return -2;
		}

        return -1;
    }

	bool IsItADraw() {
		for (int i = 0; i < ROWS; ++i) {
			for (int j = 0; j < COLS; ++j) {
				if (matrix[i][j] == 0)
					return false;
			}
		}

		return true;
	}

public:
	Connect4Logic() {
		ResetGameState();
	}

	void ResetGameState() {
		InitMatrix();
		DeterminePlayerTurn();
	}

	bool IsPlayer1Turn() {
		return currentPlayer == 1;
	}

	void SwitchPlayerTurn() {
		if (currentPlayer == 1) {
			currentPlayer = 2;
		}
		else {
			currentPlayer = 1;
		}
	}

	void GetAllPossiblePlayablePositions(int inputMatrix[ROWS][COLS], Vector2 array[COLS], int *len) {
		int counter = 0;

		for (int c = 0; c < COLS; ++c) {
			for (int r = 0; r < ROWS; ++r) {
				if (inputMatrix[r][c] == 0) {
					if (r == 0) {
						array[counter].x = c;
						array[counter].y = r;
						++counter;
					}
					else {
						if (array[counter-1].x == c) {
							--counter;
							array[counter].x = c;
							array[counter].y = r;
							++counter;
						}
					}
				}
			}
		}

		*len = counter;
	}

	Vector2* GetWiningPositions() {
		return winingPositions;
	}

	Vector2 GetBestPosition() {
		int prevMatrixState[ROWS][COLS];
		unsigned char prevPlayerTurn = currentPlayer;

		// cloning the current matrix state
		for (int i = 0; i < ROWS; ++i) {
			for (int j = 0; j < COLS; ++j) {
				prevMatrixState[i][j] = matrix[i][j];
			}
		}

		Vector2 currentPossiblePositions[COLS] = { 0 };
		int len = 0;
		GetAllPossiblePlayablePositions(matrix, currentPossiblePositions, &len);
		Vector2 bestPosition = { 0 };
		int bestMove = INT_MIN;

		for (int i = 0; i < len; ++i) {
			PlayState playState = Play(currentPossiblePositions[i], currentPlayer);
			if (playState.success) {
				currentPlayer = prevPlayerTurn;
				int currentMove = SimulateGamePlay(currentPossiblePositions[i], 2);
				UnPlay(currentPossiblePositions[i]);
				if (currentMove > bestMove) {
					bestMove = currentMove;
					bestPosition = currentPossiblePositions[i];
				}
			}
		}

		// restoring the previous matrix state
		for (int i = 0; i < ROWS; ++i) {
			for (int j = 0; j < COLS; ++j) {
				matrix[i][j] = prevMatrixState[i][j];
			}
		}

		currentPlayer = prevPlayerTurn;

		return bestPosition;
	}

	int SimulateGamePlay(Vector2 currentPossiblePlayablePosition, unsigned char currentPlayer) {
		switch(this->CheckIfSomeoneWon(currentPossiblePlayablePosition, currentPlayer)) {
		case 2:
			return 1;
		case 1:
			return -1;
		case -2:
			return 0;
		}

		if (currentPlayer == 1) {
			Vector2 currentPossiblePositions[COLS] = { 0 };
			int len = 0;
			GetAllPossiblePlayablePositions(matrix, currentPossiblePositions, &len);
			Vector2 bestPosition = { 0 };
			int bestMove = INT_MIN;

			for (int i = 0; i < len; ++i) {
				PlayState playState = Play(currentPossiblePositions[i], currentPlayer);
				if (playState.success) {
					if (playState.someoneWon) {
						switch (playState.whoWon) {
						case 1:
							return -1;
						case 2:
							return 1;
						default:
							return 0;
						}
					}
					int currentMove = SimulateGamePlay(currentPossiblePositions[i], 2);
					UnPlay(currentPossiblePositions[i]);
					if (currentMove > bestMove) {
						bestMove = currentMove;
						bestPosition = currentPossiblePositions[i];
					}
				}
			}

			return bestMove;
		}
		else {
			Vector2 currentPossiblePositions[COLS] = { 0 };
			int len = 0;
			GetAllPossiblePlayablePositions(matrix, currentPossiblePositions, &len);
			Vector2 bestPosition = { 0 };
			int bestMove = INT_MAX;

			for (int i = 0; i < len; ++i) {
				PlayState playState = Play(currentPossiblePositions[i], currentPlayer);
				if (playState.success) {
					if (playState.someoneWon) {
						switch (playState.whoWon) {
						case 1:
							return -1;
						case 2:
							return 1;
						default:
							return 0;
						}
					}
					int currentMove = SimulateGamePlay(currentPossiblePositions[i], 1);
					UnPlay(currentPossiblePositions[i]);
					if (currentMove < bestMove) {
						bestMove = currentMove;
						bestPosition = currentPossiblePositions[i];
					}
				}
			}

			return bestMove;
		}
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
			return { false, false, -1, false };
		}

		matrix[(int)inputVec.y][(int)inputVec.x] = IsPlayer1Turn() ? 1 : 2;

        int wonPlayer;
        if ((wonPlayer = CheckIfSomeoneWon(inputVec, IsPlayer1Turn() ? 1 : 2)) != -1 && wonPlayer != -2) {
            cout << "Player " << wonPlayer << " won" << endl;
            return { true, true, wonPlayer, false };
        }

		if (wonPlayer == -2) {
			return { false, true, -1, true };
		}

		return { false, true, -1, false };
	}

	PlayState Play(Vector2 inputVec, unsigned char player) {
		if (matrix[(int)inputVec.y][(int)inputVec.x] != 0) {
			return { false, false, -1, false };
		}

		matrix[(int)inputVec.y][(int)inputVec.x] = player;

		int wonPlayer;
		if ((wonPlayer = CheckIfSomeoneWon(inputVec, player)) != -1 && wonPlayer != -2) {
			cout << "Player " << wonPlayer << " won" << endl;
			return { true, true, wonPlayer, false };
		}

		if (wonPlayer == -2) {
			return { false, true, -1, true };
		}

		return { false, true, -1, false };
	}

	void UnPlay(Vector2 position) {
		matrix[(int)position.y][(int)position.x] = 0;
	}
};

