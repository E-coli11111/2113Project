#include <iostream>
#include <iomanip>
#include <conio.h>
#include <string>
#include <windows.h>

using namespace std;

void SetPos(COORD a) {
	HANDLE out=GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(out, a);
}

void SetPos(int i, int j) {
	COORD pos={i, j};
	SetPos(pos);
}

COORD position[4];
COORD centre;

void draw_people(COORD centre) {
	position[0].X = centre.X;
	position[0].Y = centre.Y - 1;
	position[1].X = centre.X;
	position[1].Y = centre.Y;
	position[2].X = centre.X - 1;
	position[2].Y = centre.Y + 1;
	position[3].X = centre.X + 1;
	position[3].Y = centre.Y + 1;
	SetPos(position[0]);
	cout << 'O';
	SetPos(position[1]);
	cout << '|';
	SetPos(position[2]);
	cout << '/';
	SetPos(position[3]);
	cout << '\\';	
}

void draw_null_people() {
	for (int i = 0; i < 4; i++) {
		SetPos(position[i]);
		cout << ' ';
	}
}

void initial_people() {
	centre.X = 1;
	centre.Y = 24;
	draw_people(centre);
}

void jump() {
	for (int i = 0; i < 5; i++) {
		draw_null_people();
		centre.Y -= 1;
		draw_people(centre);
		Sleep(100);
	}
	for (int i = 0; i < 5; i++) {
		draw_null_people();
		centre.Y += 1;
		draw_people(centre);
		Sleep(100);
	}
}


int main() {
	initial_people();
	while(1) {
		char x = getch();
		if (x = 'k') jump();
	}
}
