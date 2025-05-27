#ifndef GAME2048_H
#define GAME2048_H

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <conio.h>
#include <fstream>

// ���������� ���������� ������ �� ��������
extern HANDLE h;

enum direction { DOWN = 80, UP = 72, LEFT = 75, RIGHT = 77, ENTER = 13, ESC = 27 };

const int MENU_SIZE = 3;
extern const char* menuItems[MENU_SIZE];

const int BOX_SIZE = 4;
const int WIN_COUNT = 2048;
extern int board[BOX_SIZE][BOX_SIZE];
extern int score;

// ��������� ������� ����������� ������
void ConsoleSett();
void setCursorPos(int x, int y);
void setColor(int color);

// ��������� ������� ��� ������ � ����
void drawFrame(int x, int y, int width, int height, int color);
void drawMenu(int selected);
void menu_init();

// ��������� ������� ���
void set_board();
void add_number();
void draw_board();
void move_up();
void move_down();
void move_left();
void move_right();

// ��������� ������� ������� ��� ���������� ����� ���
bool is_full();
bool is_won();
bool no_move();

void new_game();

// ��������� ������� ����������/������������
void autosave(const char* filename);
void loadsave(const char* filename);

void loading();

#endif // GAME2048_H