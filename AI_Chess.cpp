// AI_Chess.cpp : Defines the entry point for the console application.
//

//James Cole
//Sean McQuid

#include "stdafx.h"


/*

*/

#include <utility> 
#include <list>

#include<stdio.h>
using namespace std;

#define ROWS 8
#define COLS 8

#define EMPTY 1
#define RED 2
#define BLACK 3
#define REDKING 4
#define BLACKKING 5


#define ISRED(c)  (c == RED || c == REDKING)
#define ISBLACK(c) (c == BLACK || c == BLACKKING)
#define ISEMPTY(c) (c == 1)

struct movement {
	int i = 0;
	int j = 0;
	int k = 0;
	int l = 0;
	int value = 0; //Null
};

static int REDLEFT;
static int BLACKLEFT;
static list<movement> possible;
static list<pair<int, int>> RED_locations;
static list<pair<int, int>> BLACK_locations;

static list <pair<int, int>>::iterator pi_it;

void printDisplay(int d[][COLS]);
void swapIJKL(int d[ROWS][COLS], int i, int j, int k, int l);
char value2symbol(int i);
void printDisplayFancy(int d[][COLS]);
int Playersturn(int d[][COLS], int player, int i, int j, int k, int l);
void Computersturn(int f[][COLS], int player, int i, int j, int k, int l);
list<movement> avalible_moves(int d[][COLS], int i, int j);
list<pair<int, int>> piece_locator(int d[][COLS], bool re_or_bl);
int max_min_value(int d[][COLS]);

class AI {
	int alpha = -1;
	int beta = 999;
	int max = -1;
	int min = 999;
	pair<int,int> best_move_max;
	pair<int, int> move_to_max;
	pair<int, int> best_move_min;
	pair<int, int> move_to_min;
	bool active = false;
public:
	void find_best_move(int d[][COLS], int player, int i, char j, int k, char l);
	int minimax(int d[][COLS], bool max_min, int depth, int start_depth);
	int alpha_beta(int d[][COLS], bool max_min, int depth, int start_depth);
	void activate_ai();
	bool is_active();
	pair<int, int> get_moves(bool from_to, bool max_min);
};

void AI::find_best_move(int d[][COLS], int player, int i, char j, int k, char l) {
	if (player == RED) {
		//minimax(d, true, 2, 2);
		alpha = -999;
		beta = 999;
		alpha_beta(d, true, 2, 2);
		i = best_move_max.first + 1;
		j = best_move_max.second + 'a';
		k = move_to_max.first + 1;
		l = move_to_max.second + 'a';
	}else{//Black
		//minimax(d, false, 2, 2);
		alpha = -999;
		beta = 999;
		alpha_beta(d, false, 2, 2);
		i = best_move_min.first + 1;
		j = best_move_min.second + 'a';
		k = move_to_min.first + 1;
		l = move_to_min.second + 'a';
	}
	//printf("i: %d, j: %c, k: %d, l: %c\n", i, j, k, l);
}

/*int AI::minimax(int d[][COLS], bool max_min, int depth) {
	/*if (depth == 0) {
		return;
	}

	alpha = -999;
	beta = 999;

	int value;
	int mx = -999;
	int mn = 999;

	int f[ROWS][COLS];
	memcpy(f, d, sizeof(f));

	list<pair<int, int>> piece_locations;
	list<movement> moves; //Nodes to check

						  //Gets location of pieces
	if (max_min) {
		piece_locations = piece_locator(d, true);
	}
	else {
		piece_locations = piece_locator(d, false);
	}

	list<movement> piec_moves;

	//Gets possible moves/ Nodes
	for (auto it = piece_locations.begin(); it != piece_locations.end(); ++it) {
		pair<int, int> piece = *it;
		piec_moves = avalible_moves(d, piece.first, piece.second);
		//moves.merge(piec_moves);
		moves.splice(moves.end(), piec_moves);
	}

	//Looks at each node
	//max = -999;
	//min = 999;


	for (auto it = moves.begin(); it != moves.end(); it++) {
		memcpy(f, d, sizeof(f));
		movement curr = *it;
		int i = curr.i;
		int j = curr.j;
		int k = curr.k;
		int l = curr.l;

		if (depth != 0) {
			swapIJKL(f, i, j, k, l);
			value = minimax(f, !max_min, depth - 1);
		}
		else {
			value = max_min_value(f);
		}

		if (max_min) {

			if (curr.value > max) {
				max = curr.value;
				best_move_max.first = i;
				best_move_max.second = j;
				move_to_max.first = k;
				move_to_max.second = l;
			}
		}
		else {

			if (curr.value < min) {
				min = curr.value;
				best_move_min.first = i;
				best_move_min.second = j;
				move_to_min.first = k;
				move_to_min.second = l;
			}
		}

	}

	return value;
}*/

int AI::minimax(int d[][COLS], bool max_min, int depth, int start_depth) {
	/*if (depth == 0) {
	return;
	}*/

	int value;
	int mx = -999;
	int mn = 999;

	int f[ROWS][COLS];
	//memcpy(f, d, sizeof(f));

	if (depth == 0) {
		value = max_min_value(d);
		//printf("Value is: %d\n", value);
	}
	else {

		list<pair<int, int>> piece_locations;
		list<movement> moves; //Nodes to check

							  //Gets location of pieces
		if (max_min) {
			piece_locations = piece_locator(d, true);
		}
		else {
			piece_locations = piece_locator(d, false);
		}

		list<movement> piec_moves;

		//Gets possible moves/ Nodes
		for (auto it = piece_locations.begin(); it != piece_locations.end(); ++it) {
			pair<int, int> piece = *it;
			piec_moves = avalible_moves(d, piece.first, piece.second);
			//moves.merge(piec_moves);
			moves.splice(moves.end(), piec_moves);
		}

		for (auto it = moves.begin(); it != moves.end(); it++) {
			memcpy(f, d, sizeof(f));
			movement curr = *it;
			int i = curr.i;
			int j = curr.j;
			int k = curr.k;
			int l = curr.l;

			//swapIJKL(f, i, j, k, l);
			if (max_min) {
				Computersturn(f, RED, i, j, k, l);
			}
			else {
				Computersturn(f, BLACK, i, j, k, l);
			}
			
			value = minimax(f, !max_min, depth - 1, start_depth);

			if (max_min) {
				//printf("max, depth:%d, i:%d, j:%d, k:%d, l:%d\n", depth, i+1, j+1, k+1, l+1);
				if (value > mx) {
					mx = value;
					if (depth == start_depth) {
						//printf("max\n\n\n");
						best_move_max.first = i;
						best_move_max.second = j;
						move_to_max.first = k;
						move_to_max.second = l;
					}
				}
			}
			else {
				//printf("min, depth:%d, i:%d, j:%d, k:%d, l:%d\n", depth, i, j, k, l);
				if (value < mn) {
					mn = value;
					if (depth == start_depth) {
						//printf("min\n\n\n");
						best_move_max.first = i;
						best_move_max.second = j;
						move_to_max.first = k;
						move_to_max.second = l;
					}
				}
			}

		}

		if (max_min) {
			value = mx;
		}
		else {
			value = mn;
		}

	}

	

	return value;
}

/*void AI::alpha_beta(int d[][COLS], bool max_min, int depth, int start_depth) {
	if (depth == 0) {
		return;
	}

	int f[ROWS][COLS];
	memcpy(f, d, sizeof(f));

	list<pair<int, int>> piece_locations;
	list<movement> moves; //Nodes to check

	//Gets location of pieces
	if (max_min) {
		piece_locations = piece_locator(d, true);
	}
	else {
		piece_locations = piece_locator(d, false);
	}

	list<movement> piec_moves;

	//Gets possible moves/ Nodes
	for (auto it = piece_locations.begin(); it != piece_locations.end(); ++it) {
		pair<int, int> piece = *it;
		piec_moves = avalible_moves(d, piece.first, piece.second);
		//moves.merge(piec_moves);
		moves.splice(moves.end(), piec_moves);
	}

	//Looks at each node, returns if alpha-beta
	max = -999;
	min = 999;

	for (auto it = moves.begin(); it != moves.end(); it++) {
		memcpy(f, d, sizeof(f));
		movement curr = *it;
		int i = curr.i;
		int j = curr.j;
		int k = curr.k;
		int l = curr.l;

		if (max_min) {
			printf("min, i:%d, j:%d, k:%d, l:%d\n", i, j, k, l);
			if (curr.value > max) {
				max = curr.value;
				best_move_max.first = i;
				best_move_max.second = j;
				move_to_max.first = k;
				move_to_max.second = l;
			}
		}
		else {
			printf("min, i:%d, j:%d, k:%d, l:%d\n", i, j, k, l);
			if (curr.value < min) {
				min = curr.value;
				best_move_min.first = i;
				best_move_min.second = j;
				move_to_min.first = k;
				move_to_min.second = l;
			}
		}

	}
	
	

}*/

int AI::alpha_beta(int d[][COLS], bool max_min, int depth, int start_depth) {
	//alpha = -999;
	//beta = 999;

	int value;
	int mx = -999;
	int mn = 999;

	int f[ROWS][COLS];
	//memcpy(f, d, sizeof(f));

	if (depth == 0) {
		value = max_min_value(d);
		//printf("Value is: %d\n", value);
	}
	else {

		list<pair<int, int>> piece_locations;
		list<movement> moves; //Nodes to check

							  //Gets location of pieces
		if (max_min) {
			piece_locations = piece_locator(d, true);
		}
		else {
			piece_locations = piece_locator(d, false);
		}

		list<movement> piec_moves;

		//Gets possible moves/ Nodes
		for (auto it = piece_locations.begin(); it != piece_locations.end(); ++it) {
			pair<int, int> piece = *it;
			piec_moves = avalible_moves(d, piece.first, piece.second);
			//moves.merge(piec_moves);
			moves.splice(moves.end(), piec_moves);
		}

		for (auto it = moves.begin(); it != moves.end(); it++) {
			memcpy(f, d, sizeof(f));
			movement curr = *it;
			int i = curr.i;
			int j = curr.j;
			int k = curr.k;
			int l = curr.l;

			//swapIJKL(f, i, j, k, l);
			if (max_min) {
				Computersturn(f, RED, i, j, k, l);
			}
			else {
				Computersturn(f, BLACK, i, j, k, l);
			}

			value = minimax(f, !max_min, depth - 1, start_depth);

			if (max_min) {
				//printf("max, depth:%d, i:%d, j:%d, k:%d, l:%d\n", depth, i + 1, j + 1, k + 1, l + 1);
				if (value > mx) {
					mx = value;
					if (mx > beta) {
						return mx;
					}
					if (depth == start_depth) {
						//printf("max\n\n\n");
						alpha = mx;
						best_move_max.first = i;
						best_move_max.second = j;
						move_to_max.first = k;
						move_to_max.second = l;
					}
				}
			}
			else {
				//printf("min, depth:%d, i:%d, j:%d, k:%d, l:%d\n", depth, i, j, k, l);
				if (value < mn) {
					mn = value;
					if (mn < alpha) {
						return mn;
					}
					if (depth == start_depth) {
						//printf("min\n\n\n");
						beta = mn;
						best_move_max.first = i;
						best_move_max.second = j;
						move_to_max.first = k;
						move_to_max.second = l;
					}
				}
			}

		}

		if (max_min) {
			value = mx;
		}
		else {
			value = mn;
		}

	}


	return value;

}

void AI::activate_ai() {
	active = true;
}

bool AI::is_active() {
	return active;
}

pair<int, int> AI::get_moves(bool from_to, bool max_min) {
	//from_to: true is from, false is to
	if (from_to) {
		if (max_min) {
			return best_move_max;
		}
		else {
			return best_move_min;
		}
	}
	else {
		if (max_min) {
			return move_to_max;
		}
		else {
			return move_to_min;
		}
		
	}
	//return;
}

static AI RED_AI;
static AI BLACK_AI;

int max_min_value(int d[][COLS]) {
	int value;
	int NUMREDS = 0;
	int NUMBLACKS = 0;
	int RED_KINGS = 0;
	int BLACK_KINGS = 0;

	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			if (ISRED(d[i][j])) {
				NUMREDS += 1;
			}
			if (ISBLACK(d[i][j])) {
				NUMBLACKS += 1;
			}
			if (d[i][j] == REDKING) {
				RED_KINGS += 1;
			}
			if (d[i][j] == BLACKKING) {
				BLACK_KINGS += 1;
			}
		}
	}

	//printf("REDS: %d\n", NUMREDS);
	//printf("BLACKS: %d\n", NUMBLACKS);

	/*if (NUMREDS > NUMBLACKS) {
		printf("R > B\n");
	}
	if (NUMREDS < NUMBLACKS) {
		printf("R < B\n");
	}*/

	value = ((NUMREDS * 3) + (RED_KINGS * 2)) - ((NUMBLACKS * 3) + (BLACK_KINGS * 2));

	return value;
}

list<movement> avalible_moves(int d[][COLS], int i, int j) {
	list<movement> moves;
	int piece = d[i][j];
	int up = i-1;
	int down = i+1;
	int left = j-1;
	int right = j+1;
	movement up_left;
	movement up_right;
	movement down_left;
	movement down_right;

	if (ISRED(piece)) {
		//down_left
		if (down <= COLS && left >= 0){
			//Normal
			if (ISEMPTY(d[down][left])) {
				down_left.i = i;
				down_left.j = j;
				down_left.k = down;
				down_left.l = left;
				down_left.value = 1;
				if (down == 7) {
					down_left.value += 2;
				}
				//std::printf("test_1\n");
			}
			//Jump
			else if (ISBLACK(d[down][left])) {
				if(down+1 <= COLS && left-1 >= 0){
					if (ISEMPTY(d[down + 1][left - 1])) {
						down_left.i = i;
						down_left.j = j;
						down_left.k = down+1;
						down_left.l = left-1;
						if (d[down][left] == BLACKKING) {
							down_left.value = 5;
						}
						else {
							down_left.value = 3;
						}
						if (down + 1 == 7) {
							down_left.value += 2;
						}
						//std::printf("test_2\n");
					}
				}
			}
		}
		//Down_right
		if (down <= COLS && right <= ROWS) {
			//Normal
			if (ISEMPTY(d[down][right])) {
				down_right.i = i;
				down_right.j = j;
				down_right.k = down;
				down_right.l = right;
				down_right.value = 1;
				if (down == 7) {
					down_right.value += 2;
				}
				//std::printf("test_3\n");
			}
			//Jump
			else if (ISBLACK(d[down][right])) {
				if (down + 1 <= COLS && right + 1 <= ROWS) {
					if (ISEMPTY(d[down + 1][right + 1])) {
						down_right.i = i;
						down_right.j = j;
						down_right.k = down+1;
						down_right.l = left+1;
						if (d[down][right] == BLACKKING) {
							down_right.value = 5;
						}
						else {
							down_right.value = 3;
						}
						if (down + 1 == 7) {
							down_right.value += 2;
						}
						//std::printf("test_4\n");
					}
				}
			}
		}
		//up_left
		if (up >= 0 && left >= 0 && piece == REDKING) {
			//Normal
			if (ISEMPTY(d[up][left])) {
				up_left.i = i;
				up_left.j = j;
				up_left.k = down;
				up_left.l = left;
				up_left.value = 1;
				//std::printf("test_5\n");
			}
			//Jump
			else if (ISBLACK(d[up][left])) {
				if (up - 1 >= 0 && left - 1 >= 0) {
					if (ISEMPTY(d[up - 1][left - 1])) {
						up_left.i = i;
						up_left.j = j;
						up_left.k = up - 1;
						up_left.l = left - 1;
						if (d[up][left] == BLACKKING) {
							up_left.value = 5;
						}
						else {
							up_left.value = 3;
						}
						//std::printf("test_6\n");
					}
				}
			}
		}
		//up_left
		if (up >= 0 && right <= ROWS && piece == REDKING) {
			//Normal
			if (ISEMPTY(d[up][right])) {
				up_right.i = i;
				up_right.j = j;
				up_right.k = down;
				up_right.l = left;
				up_right.value = 1;
				//std::printf("test_7\n");
			}
			//Jump
			else if (ISBLACK(d[up][right])) {
				if (up - 1 >= 0 && right + 1 <= ROWS) {
					if (ISEMPTY(d[up - 1][right + 1])) {
						up_right.i = i;
						up_right.j = j;
						up_right.k = up - 1;
						up_right.l = right + 1;
						if (d[up - 1][left - 1] == BLACKKING) {
							up_right.value = 5;
						}
						else {
							up_right.value = 3;
						}
						//std::printf("test_8\n");
					}
				}
			}
		}
	}
	else { //Black piece
		   //up_left
		if (up >= 0 && left >= 0) {
			//Normal
			if (ISEMPTY(d[up][left])) {
				up_left.i = i;
				up_left.j = j;
				up_left.k = up;
				up_left.l = left;
				up_left.value = 1;
				if (up == 0) {
					up_left.value += 2;
				}
				//std::printf("test_9\n");
			}
			//Jump
			else if (ISRED(d[up][left])) {
				if (up - 1 >= 0 && left - 1 >= 0) {
					if (ISEMPTY(d[up - 1][left - 1])) {
						up_left.i = i;
						up_left.j = j;
						up_left.k = up - 1;
						up_left.l = left - 1;
						if (d[up][left] == REDKING) {
							up_left.value = 5;
						}
						else {
							up_left.value = 3;
						}
						if (up - 1 == 0) {
							up_left.value += 2;
						}
						//std::printf("test_10\n");
					}
				}
			}
		}
		//up_right
		if (up >= 0 && right <= ROWS) {
			//Normal
			if (ISEMPTY(d[up][right])) {
				up_right.i = i;
				up_right.j = j;
				up_right.k = up;
				up_right.l = right;
				up_right.value = 1;
				if (up == 0) {
					up_right.value += 2;
				}
				//std::printf("test_11\n");
			}
			//Jump
			else if (ISRED(d[up][right])) {
				if (up - 1 >= 0 && right + 1 <= ROWS) {
					if (ISEMPTY(d[up - 1][right + 1])) {
						up_right.i = i;
						up_right.j = j;
						up_right.k = up - 1;
						up_right.l = right + 1;
						if (d[up][right] == REDKING) {
							up_right.value = 5;
						}
						else {
							up_right.value = 3;
						}
						if (up - 1 == 0) {
							up_right.value += 2;
						}
						//std::printf("test_12\n");
					}
				}
			}
		}
		//down_left
		if (down <= COLS && left >= 0 && piece == BLACKKING) {
			//Normal
			if (ISEMPTY(d[down][left])) {
				down_left.i = i;
				down_left.j = j;
				down_left.k = down;
				down_left.l = left;
				down_left.value = 1;
				//std::printf("test_13\n");
			}
			//Jump
			else if (ISRED(d[down][left])) {
				if (down + 1 <= COLS && left - 1 >= 0) {
					if (ISEMPTY(d[down + 1][left - 1])) {
						down_left.i = i;
						down_left.j = j;
						down_left.k = down + 1;
						down_left.l = left - 1;
						if (d[down][left] == REDKING) {
							down_left.value = 5;
						}
						else {
							down_left.value = 3;
						}
						//std::printf("test_14\n");
					}
				}
			}
		}
		//Down_right
		if (down <= COLS && right <= ROWS && piece == BLACKKING) {
			//Normal
			if (ISEMPTY(d[down][right])) {
				down_right.i = i;
				down_right.j = j;
				down_right.k = down;
				down_right.l = right;
				down_right.value = 1;
				//std::printf("test_15\n");
			}
			//Jump
			else if (ISRED(d[down][right])) {
				if (down + 1 <= COLS && right + 1 <= ROWS) {
					if (ISEMPTY(d[down + 1][right + 1])) {
						down_right.i = i;
						down_right.j = j;
						down_right.k = down + 1;
						down_right.l = right + 1;
						if (d[down][right] == REDKING) {
							down_right.value = 5;
						}
						else {
							down_right.value = 3;
						}
						//std::printf("test_16\n");
					}
				}
			}
		}
	}
	
	if (down_left.value != 0) {
		moves.push_back(down_left);
		//std::printf("test_17\n");
	}
	if (down_right.value != 0) {
		moves.push_back(down_right);
		//std::printf("test_18\n");
	}
	if (up_left.value != 0) {
		moves.push_back(up_left);
		//std::printf("test_19\n");
	}
	if (up_right.value != 0) {
		moves.push_back(up_right);
		//std::printf("test_20\n");
	}

	//std::printf("test_21\n");

	return moves;
}

void printDisplayFancy(int d[][COLS])
{
	int rr, cc, pp;

	std::printf("  +---+---+---+---+---+---+---+---+\n");

	for (rr = 0; rr<ROWS; ++rr)
	{
		std::printf("%d |", rr + 1);
		for (cc = 0; cc<COLS; ++cc)
		{
			std::printf(" %c |", value2symbol(d[rr][cc]));
		}
		std::printf("\n");
		std::printf("  +---+---+---+---+---+---+---+---+\n");
	}

	std::printf("    a   b   c   d   e   f   g   h\n");

	std::printf("Number of REDS left: %d", REDLEFT);
	std::printf("	");
	std::printf("Number of BLACKs left: %d\n", BLACKLEFT);
}

void swapIJKL(int d[ROWS][COLS], int i, int j, int k, int l)
{
	int temp;

	//std::printf("SWAP: %d,%d to %d,%d\n", i+1, j+1, k+1, l+1);

	temp = d[i][j];

	d[i][j] = d[k][l];

	d[k][l] = temp;


}



char value2symbol(int i)
{

	switch (i)
	{
	case 0:
		return ' ';
	case 1:
		return 'E';
	case 2:
		return 'r';
	case 3:
		return 'b';
	case 4:
		return 'R';
	case 5:
		return 'B';
	}
	return ('?');
}

int Playersturn(int d[][COLS], int player, int i, int j, int k, int l)
{
	int jmp_r;
	int jmp_c;

	if (player == RED) {
		std::printf("RED move from %d,%d to %d,%d\n", i +1, j + 1, k + 1, l +1);
	}
	else {
		std::printf("BLACK move from %d,%d to %d,%d\n", i +1, j+1, k+1, l+1);
	}

	if (i < 0 && ROWS <= i) { // keeping in bounds
		std::printf("i is out of bounds\n");
		return -1;
	}
	if (j < 0 && COLS <= j) {
		std::printf("j is out of bound");
		return -1;
	}

	if (k < 0 && ROWS <= k) {
		std::printf("k is out of bounds");
		return -1;

	}
	if (l < 0 && COLS <= l) {
		std::printf("l is out of bounds\n");
		return -1;
	}

	// check player is moving his own piece.
	if (player == RED) {
		if (d[i][j] != RED && d[i][j] != REDKING) {
			std::printf("move your own piece!\n");
			return -1;
		}
	}
	else { //BLACK
		if (d[i][j] != BLACK && d[i][j] != BLACKKING) {
			std::printf("move your own piece!\n");
			return -1;
		}
	}

	//make sure they are umping to a empty loacation
	if (d[k][l] != EMPTY) {
		std::printf("You must move to a empty location");
		return -1;
	}




	// make sure the vertical direction of the move is not illegal
	if (player == RED && d[i][j] != REDKING) {
		//make sure he moves down
		if (i >= k) {
			std::printf("RED player must move down\n");
			return -1;
		}
	}
	else if (player == BLACK && d[i][j] != BLACKKING) { //BLACK
		   // make sure player moves up
		if (i <= k) {
			std::printf("BLACK player must move up\n");
			return -1;
		}
	}


	// check if it's a regular move
	if (i - k == -1 || i - k == 1) {
		if (j - l == 1 || j - l == -1) {
			swapIJKL(d, i, j, k, l);
			//Check if piece is kinged
			if (k == 7 && d[k][l] == RED) {
				d[k][l] = REDKING;
			}
			if (k == 0 && d[k][l] == BLACK) {
				d[k][l] = BLACKKING;
			}
			return 0;
		}
	}

	//check if it's a jump move
	if (i - k == -2 || i - k == 2) {
		if (j - l == -2 || j - l == 2) {
			// check if the jump position is enemy
			if (i < k) { // move down
				jmp_r = i + 1;
				std::printf("i down is %d\n", j);
			}
			else { // move up
				jmp_r = i - 1;
				std::printf("i up is %d\n", j);
			}
			if (j < l) { // move down
				jmp_c = j + 1;
				std::printf("j down is %d\n", j);
				//jmp_c = j;
			}
			else { // move up
				std::printf("j up is %d\n", j);
				jmp_c = j - 1;
				//jmp_c = j;
			}

			if ((player == RED || player == REDKING)
				&& (d[jmp_r][jmp_c] != 3 && d[jmp_r][jmp_c] != 5))
			//if (player == RED && d[jmp_r][jmp_c] != BLACK)
			{
				std::printf("Player: %d\n", player);
				std::printf("%d\n", d[jmp_r][jmp_c]);
				std::printf("%d\n", d[jmp_c][jmp_r]);
				std::printf("Enemy is not Black at %d,%d\n", jmp_r+1, jmp_c+1);
				return -1;
			}
			/*(player == BLACK || player == BLACKKING)*/
			//(d[jmp_r][jmp_c] != RED || d[jmp_r][jmp_c] != REDKING)) {
			if (/*ISBLACK(player)*/(player == BLACK || player == BLACKKING) 
				&& !ISRED(d[jmp_r][jmp_c])){
			//if (player == BLACK && d[jmp_r][jmp_c] != RED)
				std::printf("%d\n", d[jmp_r][jmp_c]);
				std::printf("%d\n", d[jmp_c][jmp_r]);
				std::printf("Enemy is not Red at %d,%d\n", jmp_r+1, jmp_c+1);
				return -1;
			}

			// we are sure the move is valid:
			if (ISBLACK(d[jmp_r][jmp_c])) {
				BLACKLEFT--;
			}

			if (ISRED(d[jmp_r][jmp_c])) {
				REDLEFT--;
			}
			d[jmp_r][jmp_c] = 1;
			swapIJKL(d, i, j, k, l);
			//Check if piece is kinged
			if (k == 7 && d[k][l] == RED) {
				d[k][l] = REDKING;
			}
			if (k == 0 && d[k][l] == BLACK) {
				d[k][l] = BLACKKING;
			}
			return 0;
		}
	}


	std::printf("You can only move diagnally\n");
	return -1;

}

void Computersturn(int f[][COLS], int player, int i, int j, int k, int l) {
	int jmp_r;
	int jmp_c;

	// check if it's a regular move
	if (i - k == -1 || i - k == 1) {
		if (j - l == 1 || j - l == -1) {
			swapIJKL(f, i, j, k, l);
			//Check if piece is kinged
			if (k == 7 && f[k][l] == RED) {
				f[k][l] = REDKING;
			}
			if (k == 0 && f[k][l] == BLACK) {
				f[k][l] = BLACKKING;
			}
			return;
		}
	}

	//check if it's a jump move
	if (i - k == -2 || i - k == 2) {
		if (j - l == -2 || j - l == 2) {
			// check if the jump position is enemy
			if (i < k) { // move down
				jmp_r = i + 1;
			}
			else { // move up
				jmp_r = i - 1;
			}
			if (j < l) { // move down
				jmp_c = j + 1;
			}
			else { // move up
				jmp_c = j - 1;
			}

			f[jmp_r][jmp_c] = 1;
			swapIJKL(f, i, j, k, l);
			//Check if piece is kinged
			if (k == 7 && f[k][l] == RED) {
				f[k][l] = REDKING;
			}
			if (k == 0 && f[k][l] == BLACK) {
				f[k][l] = BLACKKING;
			}
			return;
		}
	}
}

list<pair<int, int>> piece_locator(int d[][COLS], bool re_or_bl) {
	//re_or_bl:True for red, false for black
	list<pair<int, int>> locations;
	int r, c;
	for (r = 0; r < ROWS; r++) {
		for (c = 0; c < COLS; c++) {
			if (ISRED(d[r][c]) && re_or_bl) {
				pair<int, int> red_pair(r, c);
				locations.push_back(red_pair);
			}
			if (ISBLACK(d[r][c]) && !re_or_bl) {
				pair<int, int> black_pair(r, c);
				locations.push_back(black_pair);
			}
		}
	}
	return locations;
}

int main()
{
	int r, c;
	int pr, pb;
	int i, k;
	char j, l;
	REDLEFT = 12;
	BLACKLEFT = 12;

	int d[ROWS][COLS] = {
		{ 0,2,0,2,0,2,0,2 },
		{ 2,0,2,0,2,0,2,0 },
		{ 0,2,0,2,0,2,0,2 },
		{ 1,0,1,0,1,0,1,0 },
		{ 0,1,0,1,0,1,0,1 },
		{ 3,0,3,0,3,0,3,0 },
		{ 0,3,0,3,0,3,0,3 },
		{ 3,0,3,0,3,0,3,0 } };

	while (1) {
		std::printf("Select players:\n");
		std::printf("1: Two players\n");
		std::printf("2: RED player, BLACK computer\n");
		std::printf("3: BLACK player, RED computer\n");
		std::printf("4: Two computers\n");
		std::printf("Select: ");
		std::scanf("%d", &pr);
		if (pr == 1) {

			break;
		}
		if (pr == 2) {
			BLACK_AI.activate_ai();
			break;
		}
		if (pr == 3) {
			RED_AI.activate_ai();
			break;
		}
		if (pr == 4) {
			BLACK_AI.activate_ai();
			RED_AI.activate_ai();
			break;
		}
		std::printf("Select 1, 2, 3 or 4\n");
	}

	/*RED_locations = piece_locator(d, true);
	BLACK_locations = piece_locator(d, false);
	std::printf("RED locations\n");
	for (pi_it = RED_locations.begin(); pi_it != RED_locations.end(); pi_it++) {
		pair<int, int> loc = *pi_it;
		std::printf("%d,%d ", loc.first + 1, loc.second + 1);
	}
	std::printf("\n");
	std::printf("BLACK locations\n");
	for (pi_it = BLACK_locations.begin(); pi_it != BLACK_locations.end(); pi_it++) {
		pair<int, int> loc = *pi_it;
		std::printf("%d,%d ", loc.first + 1, loc.second + 1);
	}
	std::printf("\n");*/

	for (;;) {
		// display board
		printDisplayFancy(d);
		std::scanf("Press Enter");
		// red turn
		while (1) {
			if (RED_AI.is_active()) {
				std::printf("RED AI's turn\n");
				RED_AI.find_best_move(d, RED, i, j, k, l);
				pair<int, int> coords = RED_AI.get_moves(true, true);
				i = coords.first + 1; j = coords.second + 'a';
				coords = RED_AI.get_moves(false, true);
				k = coords.first + 1; l = coords.second + 'a';
				printf("RED AI chooses: %d%c to %d%c\n", i, j, k, l);
				//RED_AI.minimax(d, true, 2);
			}
			else {
				std::printf("Red's turn, choose a piece: ");
				std::scanf("%d%c", &i, &j);
				possible = avalible_moves(d, i - 1, j - 'a');
				/*list<movement>::iterator it;
				for (it = possible.begin(); it != possible.end(); ++it) {
				movement move_test = *it;
				std::printf("%d,%d\n", move_test.k + 1, move_test.l + 1);
				std::printf("Value: %d\n", move_test.value);
				}*/

				std::printf("to: ");
				std::scanf("%d%c", &k, &l);
			}
			
			//printf("i: %d, j: %c, k: %d, l: %c\n", i, j, k, l);

			//scanf("%d%c", &i, &j);
			//std::scanf("%d%c", &k, &l);

			//swapIJKL(d,i-1,j - 'a',k-1,l - 'a');
			if (Playersturn(d, RED, i - 1, j - 'a', k - 1, l - 'a') == 0)
				break;
			std::printf("Illegal move, try again\n");
			scanf("enter");
		}

		//check for win
		if (BLACKLEFT == 0) {
			std::printf("Red Wins!!\n");
			break;
		}

		//display board
		printDisplayFancy(d);
		std::scanf("Press Enter");
		//black turn
		while (1) {

			//std::printf("Black's turn: ");
			if (BLACK_AI.is_active()) { //AI goes
				std::printf("BLACK AI's turn\n");
				BLACK_AI.find_best_move(d, BLACK, i, j, k, l);
				pair<int, int> coords = BLACK_AI.get_moves(true, true);
				i = coords.first + 1; j = coords.second + 'a';
				coords = BLACK_AI.get_moves(false, true);
				k = coords.first + 1; l = coords.second + 'a';
				printf("BLACK AI chooses: %d%c to %d%c\n", i, j, k, l);
			}
			else { //Human player
				std::scanf("%d%c", &i, &j);
				possible = avalible_moves(d, i - 1, j - 'a');
				std::printf("to :");
				std::scanf("%d%c", &k, &l);
			}
			
			//swapIJKL(d,i-1,j - 'a',k-1,l - 'a');
			if (Playersturn(d, BLACK, i - 1, j - 'a', k - 1, l - 'a') == 0)
				break;
			std::printf("Illegal move, try again\n");
			scanf("enter");
		}

		//check for win
		if (REDLEFT == 0) {
			std::printf("Black Wins!!\n");
			break;
		}
		

	}

	getchar();
	getchar();
	return 0;
}



/*


if(player == RED){
if(i - k == -1){
if(j - l == 1 || j - l == -1){
if(d[k][l]==EMPTY){
swapIJKL(d,i,j,k,l);
return 0;
} else {
std::printf("Red must move to a empty location\n");
return -1;
}
}
}
} else { //BLACK
if(i - k == 1){
if(j - l == 1 || j - l == -1){
if(d[k][l]==EMPTY){
swapIJKL(d,i,j,k,l);
return 0;
} else {
std::printf("Black must move to a empty location\n");
return -1;
}
}
}
}

*/

