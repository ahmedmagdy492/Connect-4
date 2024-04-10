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

        return -1;
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

	void GetAllPossiblePlayablePositions(Vector2 array[COLS], int *len) {
		int counter = 0;

		for (int c = 0; c < COLS; ++c) {
			for (int r = 0; r < ROWS; ++r) {
				if (matrix[r][c] == 0) {
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
			return { false, false, -1 };
		}

		matrix[(int)inputVec.y][(int)inputVec.x] = IsPlayer1Turn() ? 1 : 2;

        int wonPlayer;
        if ((wonPlayer = CheckIfSomeoneWon(inputVec, IsPlayer1Turn() ? 1 : 2)) != -1) {
            cout << "Player " << wonPlayer << " won" << endl;
            return { true, true, wonPlayer };
        }

		return { false, true, -1 };
	}
};

