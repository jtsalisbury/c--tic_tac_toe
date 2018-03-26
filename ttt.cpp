#include <string>
#include <vector>
#include <bitset>
#include <fstream>
#include <iostream>
#include <locale>
#include <cstdlib>

using namespace std;

class ttt {
	private: 
		int rows, columns, winCount, player, winner;
		int board[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
		bool isplayer, debugMode;

	public:
		ttt(bool isPlayer) {
			isplayer = isPlayer;
			rows = 3, columns = 3;
			winCount = 3;
			
			player = 1;
			winner = 0;
			debugMode = false;
		}

		// Set the debug mode on in order to print AI calls
		void setDebugMode() {
			debugMode = true;
		}

		// Get the current player
		int getPlayer() {
			return player;
		}

		// Sets the space on the board to the player who's turn it is
		bool setSpace(int row, int column) {
			if (row > rows || column > columns) {
				return false;
			}

			if (row < 1 || column < 1) {
				return false;
			}

			if (board[row - 1][column - 1] != 0) {
				return false; 
			}

			board[row - 1][column - 1] = player;
			return true;
		}

		// Returns whether or not the game should end and a winner should be proclaimed.
		bool shouldWin() {

			// Win in a row
			int toLeft, toRight, middle, tot;
			for (int row = 0; row < rows; row++) {
				
				toLeft = board[row][0];
				toRight = board[row][2];
				middle = board[row][1];

				if (toLeft == toRight && toRight == middle) {

					tot = toLeft + toRight + middle;
					
					if (tot == 3) {

						winner = 1;
						return true;
					} else if (tot == 6) {

						winner = 2; 
						return true;
					}
				}
			}

			// Win in a column
			int up, down;
			for (int col = 0; col < columns; col++) {
				up = board[0][col];
				down = board[2][col];
				middle = board[1][col];

				if (up == down && down == middle) {

					tot = up + down + middle;
					if (tot == 3) {

						winner = 1;
						return true;
					} else if (tot == 6) {

						winner = 2;
						return true;
					}
				}
			}

			// Win in diagonals
			int upLeft, upRight, btmLeft, btmRight, mid;

			mid = board[1][1];

			upLeft = board[0][0];
			btmRight = board[2][2];

			btmLeft = board[2][0];
			upRight = board[0][2];

			if (mid == upLeft && upLeft == btmRight) {

				tot = mid + upLeft + btmRight;

				if (tot == 3) {

					winner = 1;
					return true;
				} else if (tot == 6) {
				
					winner = 2;
					return true;
				}
			}

			if (mid == btmLeft && btmLeft == upRight) {

				tot = mid + btmLeft + upRight;
				if (tot == 3) {

					winner = 1;
					return true;
				} else if (tot == 6) {

					winner = 2;
					return true;
				}
			}

			// Board is full, no winner so return 3 for draw
			bool isBoardFull = true;
			for (int r = 0; r < 3; r++) {
				for (int c = 0; c < 3; c++) {
					if (board[r][c] == 0) {
						isBoardFull = false;
					}
				}
			}

			if (isBoardFull) {
				winner = 3;
			}

			return isBoardFull;
		}

		// Returns the player who won the game or 0 if no winner
		int getWinner() {
			return winner;
		}

		// Print board prints the board out to the user's command line
		void printBoard() {
			char type;

			for (int row = 0; row < rows; row++) {
				for (int col = 0; col < columns; col++) {
					if (board[row][col] == 1) {
						type = 'x';
					} else if (board[row][col] == 2) {
						type = 'o';
					} else {
						type = ' ';
					}

					if (col == 2) {
						cout << type << endl;
						continue;
					}

					cout << type << " | ";
				}

				if (row == 2) {
					return;
				}

				cout << "----------" << endl;
			}
		}

		// Returns whether or not the game is PvP or PvAI
		bool isPlayerBased() {
			return isplayer;
		}

		// Updates the player whose turn it is.
		void setPlayer(int ply) {
			player = ply;
		}

		// Resets the player to 1 and all board positions to 0.
		void resetBoard() {
			player = 1;

			for (int r = 0; r < 3; r++) {
				for (int c = 0; c < 3; c++) {
					board[r][c] = 0;
				}
			}
		}

		// Computes where the AI should play 
		void getAIData(int arr[], int len) {
			
			// Play a spot in a row
			int left, mid, right, top, btm, upRight, upLeft, btmLeft, btmRight;
			for (int row = 0; row < 3; row++) {
				left = board[row][0];
				mid  = board[row][1];
				right = board[row][2];

				// Play in the last position
				if (left != 0 && left == mid && right == 0) {
					arr[0] = row + 1;
					arr[1] = 3;
					
					if (debugMode)
						cout << "P1" << endl;
					
					return;

					// Play in the middle position
				} else if (mid == 0 && left != 0 && left == right) {
					arr[0] = row + 1;
					arr[1] = 2;
					
					if (debugMode)
						cout << "P2" << endl;
					
					return; 

					// Play in the left position
				} else if (left == 0 && mid != 0 && mid == right) {
					arr[0] = row + 1;
					arr[1] = 1;

					if (debugMode)
						cout << "P3" << endl;

					return;
				}
			}

			// Play in a column
			for (int col = 0; col < 3; col++) {
				top = board[0][col];
				mid = board[1][col];
				btm = board[2][col];

				// Play in the bottom
				if (btm == 0 && top != 0 && top == mid) {
					arr[0] = 3;
					arr[1] = col + 1;

					if (debugMode)
						cout << "P4" << endl;
					
					return;

					// Play in the middle
				} else if (mid == 0 && top != 0 & top == btm) {
					arr[0] = 2;
					arr[1] = col + 1;

					if (debugMode)
						cout << "P5" << endl;

					return; 

					// Play in the top
				} else if (top == 0 && mid != 0 && mid == btm) {
					arr[0] = 1;
					arr[1] = col + 1;

					if (debugMode)
						cout << "P6" << endl;
					
					return;
				}
			}

			// Play in a diagonal position
			upRight = board[0][2];
			upLeft  = board[0][0];
			btmLeft = board[2][0];
			btmRight = board[2][2];
			mid = board[1][1];

			// Play in bottom right
			if (upLeft != 0 && upLeft == mid && btmRight == 0) {
				arr[0] = 3;
				arr[1] = 3;

				if (debugMode)
					cout << "P7" << endl;
				
				return;

				// Play in middle
			} else if (upLeft != 0 && upLeft == btmRight && mid == 0) {
				arr[0] = 2;
				arr[1] = 2;

				if (debugMode)
					cout << "P8" << endl;
				
				return;

				// Play in upper left
			} else if(upLeft == 0 && mid != 0 && mid == btmRight) {
				arr[0] = 1;
				arr[1] = 1;

				if (debugMode)
					cout << "P9" << endl;
				
				return;
			}

			// Play in bottom left
			if (upRight != 0 && upRight == mid && btmLeft == 0) {
				arr[0] = 3;
				arr[1] = 1;

				if (debugMode)
					cout << "P10" << endl;
				
				return;

				// Play in middle
			} else if (upRight != 0 && mid == 0 && btmLeft == upRight) {
				arr[0] = 2;
				arr[1] = 2;

				if (debugMode)
					cout << "P11" << endl;
				
				return;

				// Play in upper right
			} else if (upRight == 0 && mid != 0 && btmLeft == mid) {
				arr[0] = 1;
				arr[1] = 3;

				if (debugMode)
					cout << "P12" << endl;
				
				return;
			}

			// Don't need to block a spot or put ourselves in a place to when? Then choose a random spot.
			int r, c;
			int min = 0, max = 2;

			srand(time(NULL));

			do {
				r = rand() % (max - min + 1) + min;
				c = rand() % (max - min + 1) + min;
			} while (board[r][c] != 0);

			arr[0] = r + 1;
			arr[1] = c + 1;
			
			// Choose the next open spot in the board going from row -> column.
			/*for (int r = 0; r < 3; r++) {
				for (int c = 0; c < 3; c++) {
					if (board[r][c] == 0) {
						arr[0] = r + 1;
						arr[1] = c + 1;

						if (debugMode)
							cout << "P13" << endl;
						
						return;
					}
				}
			}*/
		}
};

int main() {
	int isPlayerBased;

	do {
		cout << "Hi! Will you be playing with another person or against the computer?" << endl << "Enter 0 for PVP or 1 for AI: ";
		cin >> isPlayerBased;
	} while (isPlayerBased != 1 && isPlayerBased != 0);

	// Construct the new object.
	ttt handle(isPlayerBased == 0);
	handle.setDebugMode();

	int row, col, nextPly;
	int continuePlaying = 1;
	int aiData[2];

	do {

		do {

			cout << endl << "---------------------------------------------" << endl << endl;

			handle.printBoard();

			// Handle getting inputs for where the player wants to play.
			do {
				if (!handle.isPlayerBased() && handle.getPlayer() == 2) {
					handle.getAIData(aiData, 2);

					cout << "Player AI it is your turn!" << endl;
					cout << "Where would you like to play? (row col, both must be less than 4): ";
					cout << aiData[0] << " " << aiData[1];

					row = aiData[0];
					col = aiData[1];

				} else {
					cout << "Player " << handle.getPlayer() << " it is your turn!" << endl;
					cout << "Where would you like to play? (row col, both must be less than 4): ";
				
					cin >> row >> col;
				}

			} while(!handle.setSpace(row, col));

			// Update the next player
			if (handle.getPlayer() == 1) {
				nextPly = 2;
			} else {
				nextPly = 1;
			}

			handle.setPlayer(nextPly);

		} while (!handle.shouldWin());

		cout << endl;
		handle.printBoard(); 
		cout << endl;

		// Game ending, want to play again?
		if (handle.getWinner() == 3) {
			cout << "This game was a draw! Would you like to play again?" << endl;
		} else {
			cout << "Congratulations! Player " << handle.getWinner() << " has won!" << endl;
		}

		cout << "Would you like to play again? (1 = yes, else = no): ";

		cin >> continuePlaying;

		handle.resetBoard();


	} while(continuePlaying == 1);

	return 0;
}
