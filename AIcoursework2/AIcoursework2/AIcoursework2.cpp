#include "pch.h"
#include<stdio.h>
#include<string>
#include<windows.h>
#include<iostream>

using namespace std;
#define N 15 //size of board
#define sameColor same(row + dx[u] * i, col + dy[u] * i, key)//Check whether its the same color
#define SPD for (i = 1; sameColor; i++)sumNum++; //Search positive direction
#define SND for (i = -1; sameColor; i--)sumNum++;//Search negative direction
#define checkVaildPoint if(!inboard(row + dx[u] * i, col + dy[u] * i) || Board[row + dx[u] * i][col + dy[u] * i] != 0)continue;//Check whether the position is vaild
#define s_Computer 1
int nodeNum = 0;//Record total number of expanded nodes
int TToNN = 0;//Record in this turn how many nodes expanded
int Board[N + 2][N + 2]; //use a 2-dimensional array to store a board
int s = 0;//determine whose turn now
int s0 = 0;//determine who plays first
bool boolEnd = false;//distinguish whether the game is end

//use these two array to represent the positions surrounds a surround.
//For example,as for positon (row,col),the right position of a position is (row+dx[0],col+dy[0]).
//8 means 8 directions of a position
int dx[8] = { 1, 1, 0, -1, -1, -1, 0, 1 }; 
int dy[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };

int printSymbol(int i, int j)
{
	if (Board[i][j] == 1)return printf("○");
	if (Board[i][j] == 2)return printf("●");
	if (Board[i][j] == -1)return printf("△");
	if (Board[i][j] == -2)return printf("▲");
	if (i == N)
	{
		if (j == 1)return printf("┏ ");
		if (j == N)return printf("┓");
		return printf("┯ ");
	}
	if (i == 1)
	{
		if (j == 1)return printf("┗ ");
		if (j == N)return printf("┛");
		return printf("┷ ");
	}
	if (j == 1)return printf("┠ ");
	if (j == N)return printf("┨ ");
	return printf("┼ ");
}
// Drawing board
void DrawBoard()
{
	system("cls");
	int row = 0, col = 0, keyr = 0, keyc = 0;
	char alpha = 'A';
	cout<<"\n\n\n     ";
	for (col = 1; col <= N; col++)
		cout<<alpha++<<" ";
	for (row = N; row >= 1; row--)
	{
		printf("\n   %2d", row);
		for (col = 1; col <= N; col++)
		{
			printSymbol(row, col);
			if (Board[row][col] < 0) {
				keyr = row;
				keyc = col;
			}
		}
		cout<<row;
	}
	alpha = 'A';
	printf("\n     ");
	for (col = 1; col <= N; col++)
		cout<<alpha++<<" ";
	printf("\n\n");
	if (s0 == s_Computer)
		cout<<"The computer is black, the player is white"<<endl;
	else
		cout<<"The computer is white, the player is black"<<endl;
	alpha = 'A';
	if (keyr)
		cout<<"The final position is："<<(char)(alpha + keyc - 1)<<keyr<<endl;
}
//initialization
void init()
{
	//define the background and word color of command
	system("color 0f");
	cout << "input 1 or 2 to choose:" << endl << "1.Computer is black and plays firstly" << endl << "2.Player is black and plays firstly" << endl;
	cin >> s;
	if (s != 1 && s != 2) {
		cout << "please input a vaild number.";
		return init();
	}
	//ensure who plays firstly
	s0 = s;
	//use space surrounds the board
	for (int i = 0; i <= N + 1; i++)
		for (int j = 0; j <= N + 1; j++)
			Board[i][j] = 0;
	DrawBoard();
}
//check whether the position is included in the board
bool inboard(int row, int col)
{
	if (row <1 || row > N)
		return false;
	return col >= 1 && col <= N;
}
//Check whether the position is vaild to under a pawn. 
bool ok(int row, int col)
{
	return inboard(row, col) && (Board[row][col] == 0);
}

//distinguish the color of the specific position.
//For the parameter key,1 means computer,2 means player
int same(int row, int col, int key)
{
	//determine whether the position is in board
	if (!inboard(row, col))
		return false;
	return (Board[row][col] == key || Board[row][col] + key == 0);
}
//for position(row,col),computer the sum number of pawn in direction u.
//about direction u,1 means →，2 means ↗,3 means ↑， 4 means ↖,5 means ←,6 means ↙,7 means↓,8 means ↘.
int num(int row, int col, int u)
{
	int i = row + dx[u], j = col + dy[u], sum = 0, ref = Board[row][col];
	if (ref == 0)
		return 0;
	while (same(i, j, ref))
		sum++, i += dx[u], j += dy[u];
	return sum;
}
//Compute the number of open four
int openFour(int row, int col)
{
	int key = Board[row][col], sum = 0, i, u;
	//Search four directions (left and right, up and down, left up and right down, left down and right up)
	for (u = 0; u < 4; u++)
	{
		int sumNum = 1;
		for (i = 1; sameColor; i++)
			sumNum++;
		if (!inboard(row + dx[u] * i, col + dy[u] * i) || Board[row + dx[u] * i][col + dy[u] * i] != 0)
			continue;
		for (i = -1; sameColor; i--)
			sumNum++;
		if (!inboard(row + dx[u] * i, col + dy[u] * i) || Board[row + dx[u] * i][col + dy[u] * i] != 0)
			continue;
		if (sumNum == 4)sum++;
	}
	return sum;
}
//compute the number of half open four
int halfOpenFour(int row, int col)
{
	int key = Board[row][col], sum = 0, i, u;
	for (u = 0; u < 8; u++)
	{
		int  sumNum = 0;
		bool flag = true;
		//Traversing 8 directions
		for (i = 1; sameColor || flag; i++)
		{
			if (!sameColor)
			{
				if (flag&&Board[row + dx[u] * i][col + dy[u] * i])
					sumNum -= 10;
				flag = false;
			}
			sumNum++;
		}
		if (!inboard(row + dx[u] * --i, col + dy[u] * i))
			continue;
		SND
		if (sumNum == 4)
			sum++;
	}
	return sum - openFour(row, col) * 2;
}
//compute the number of open three
int openThree(int row, int col)
{
	int key = Board[row][col], sum = 0, i, u;
	//Search four directions
	for (u = 0; u < 4; u++)
	{
		int sumNum = 1;
		SPD checkVaildPoint i++; checkVaildPoint;
		SND checkVaildPoint i++; checkVaildPoint;
		if (sumNum == 3)
			sum++;
	}
	//Search 8 directions
	for (u = 0; u < 8; u++)//8个方向，每个方向最多1个非三连的活三
	{
		int  sumNum = 0;
		bool flag = true;
		for (i = 1; sameColor || flag; i++)//成活四点的方向
		{
			if (!sameColor)
			{
				if (flag&&Board[row + dx[u] * i][col + dy[u] * i])
					sumNum -= 10;
				flag = false;
			}
			sumNum++;
		}checkVaildPoint
			if (Board[row + dx[u] * --i][col + dy[u] * i] == 0)
				continue;
		SPD checkVaildPoint
			if (sumNum == 3)sum++;
	}
	return sum;
}
//Check whether there is overline
bool overline(int row, int col)
{
	bool flag = false;
	int u;
	for (u = 0; u < 4; u++)
		if (num(row, col, u) + num(row, col, u + 4) > 4)
			flag = true;
	return flag;
}
//Check disallowed moves after undering a pawn
bool disallowedMove(int row, int col)
{
	//Check the color of  player, there is no limitation for white.
	if (same(row, col, 2))
		return false;
	bool flag = openThree(row, col) > 1 || overline(row, col) || openFour(row, col) + halfOpenFour(row, col) > 1;
	return flag;
}
//Check whether the game is end after undering a pawn on (row,col)
bool gameEnd(int row, int col)
{
	int u;
	for (u = 0; u < 4; u++)
		if (num(row, col, u) + num(row, col, u + 4) >= 4)
			boolEnd = true;
	if (boolEnd)
		return true;
	boolEnd = disallowedMove(row, col);
	return boolEnd;
}
//under a pawn
void go(int row, int col)
{
	if (s == s0)
		Board[row][col] = -1; //标出最新下的棋
	else 
		Board[row][col] = -2;
	//Change the old △ to ○.
	for (int i = 0; i <= N; i++)
		for (int j = 0; j <= N; j++) //取消上一个最新棋的标识
		{
			//check whether the pawn is undered in this turn
			if (i == row && j == col)
				continue;
			if (Board[i][j] < 0)
				Board[i][j] *= -1;
		}
	DrawBoard();
	if (disallowedMove(row, col))
	{
		if (s0 == s_Computer)
			cout<<"Winner is you";
		else 
			cout<<"Winner is computer";
		Sleep(10000);
	}
	if (gameEnd(row, col))
	{
		if (s == s_Computer)
			cout<<"Winner is computer";
		else 
			cout<<"Winner is player";
		Sleep(10000);
	}
}
//compute the score
int computeScore(int row, int col)
{
	//Give disallowed moves zero
	if (disallowedMove(row, col))
		return 0;
	if (gameEnd(row, col))
	{
		boolEnd = false;
		return 10000;
	}
	int score = openFour(row, col) * 1000 + (halfOpenFour(row, col) + openThree(row, col)) * 100;
	//search whether there is a pawn surrounding the position
	for (int i = 0; i < 8; i++)
		if (Board[row + dx[i]][col + dy[i]])
			score++;
	return score;
}

int Max2(int B)
{
	int i, j;
	int beta = B;
	int alpha = -100000;
	
	for (i = 1; i <= N; i++)
		for (j = 1; j <= N; j++)
		{
			nodeNum++;
			TToNN++;
			//check whether the position is avaliable
			if (!ok(i, j))
				continue;
			//put piece on the position
			Board[i][j] = s0;
			int pScore = computeScore(i, j);
			//skip invaild position(disallowed moves' position or 
			//the position who has no neighbor chess piece)
			if (pScore == 0)
			{
				Board[i][j] = 0;
				continue;
			}
			//detemine if game can be end
			if (pScore == 10000){
				Board[i][j] = 0;
				return 10000;
			}
			Board[i][j] = 0;
			//find the maximum value
			if (pScore> alpha)
				alpha = pScore;
			//Alpha-beta pruning 
			if (alpha > beta)
				return alpha;
		}
	return alpha;
}
int Min(int A)
{
	int i, j;
	int alpha = A;
	int beta = 100000;
	for (i = 1; i <= N; i++)
		for (j = 1; j <= N; j++){
			nodeNum++;
			TToNN++;
			if (!ok(i, j))
				continue;
			Board[i][j] = 3 - s0;
			int pScore = computeScore(i, j);
			//skip invaild position(disallowed moves' position or 
			//the position who has no neighbor chess piece)
			if (pScore == 0)
			{
				Board[i][j] = 0;
				continue;
			}
			//detemine if game can be end
			if (pScore == 10000){
					Board[i][j] = 0;
					return -10000;
			}
			pScore = Max2(beta);
			Board[i][j] = 0;
			//Find the min value
			if (pScore < beta)
				beta = pScore;
			//Alpha-beta pruning 
			if (alpha > beta)
				return beta;
		}
	return beta;
}
void Max1()
{
	TToNN = 0;
	nodeNum++;
	TToNN++;
	DrawBoard();
	cout<<"It is computer's turn，please wait： ";
	//if the center position of the board is availible,under a pawn on there
	if (Board[8][8] == 0)
		return go(8, 8);
	int i, j;
	int alpha = -100000, keyi, keyj;
	for (i = 1; i <= N; i++)
	{
		for (j = 1; j <= N; j++)
		{
			nodeNum++;
			TToNN++;
			if (!ok(i, j))
				continue;
			Board[i][j] = s0;
			int pScore = computeScore(i, j);
			//skip invaild position(disallowed moves' position or 
			//the position who has no neighbor chess piece)
			if (pScore == 0)
			{ 
				Board[i][j] = 0; 
				continue; 
			}
			//detemine if game can be end
			if (pScore == 10000)
				return go(i, j);
			pScore = Min(alpha);
			Board[i][j] = 0;
			//Find the max value
			if (pScore > alpha) {
				alpha = pScore;
				keyi = i;
				keyj = j;
			}
		}
	}
	return go(keyi, keyj);
}
//Actions of player in every turn
void player()
{
	DrawBoard();
	cout << "All of the expanded nodes number is " << nodeNum << "." << endl;
	cout << "In this turn,the expanded nodes number is " << TToNN << "." << endl;
	cout<<"It is your turn，please input your position ： ";
	char c = '\n';
	int row = 0, col = 0;
	while (c < '0') {
		cin >> c;
		cin >> row;
	}
	//determine whether the first input value is lowercase
	if (c < 'a')
		col = c - 'A' + 1;
	else col = c - 'a' + 1;
	//determine whether the input position is vaild
	if (!ok(row, col)||c-'a'<0||c-'z'>0)
	{
		cout<<"It is not a vaild position";
		Sleep(1000);
		return player();
	}
	//under a pawn on the position
	go(row, col);
}
int main()
{
	init();
	while (!boolEnd)
	{
		//determine whether this turn should be played by computer
		if (s == s_Computer)
			Max1();
		else 
			player();
		//change the game turn
		s =3-s;
	}
	return 0;
}
