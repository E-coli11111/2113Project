#include <iostream>
#include <iomanip>
#include <pthread.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

//#include <windows.h>

using namespace std;

time_t start[2] = {0, 0};

int getTime() {
  return clock()/CLOCKS_PER_SEC;
}

bool Timer(time_t time_period, int id) {
  time_t end = getTime();
    if (end - start[id] >= time_period) {
		start[id] = end;
		return 1;
	}
	return 0;
}

bool _kbhit() {
  struct termios oldt, newt;
  int ch;
  int oldf;
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
  ch = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);
  if (ch != EOF) {
    ungetc(ch, stdin);
    return 1;
  }
  return 0;
}

char _getch () {
  char ch = getchar();
  return ch;
}

struct COORD{
	int X;
	int Y;
};

void SetPos(int x, int y){
    printf("\033[%d;%dH", y, x);
}

//set cursor position
void SetPos(COORD a) {
	//HANDLE out=GetStdHandle(STD_OUTPUT_HANDLE);
	//SetConsoleCursorPosition(out, a);
	SetPos(a.X, a.Y);
}

void MovePos(COORD a, string direction, int n) {
  if (direction == "up") {
    printf("\033[%dA", n);
  } else if (direction == "down") {
    printf("\033[%dB", n);
  } else if (direction == "left") {
    printf("\033[%dC", n);
  } else if (direction == "right") {
    printf("\033[%dD", n);
  }
}
  
//draw the ground
void draw_ground() {
	SetPos(0, 23);
	for (int i = 0; i <= 100; i++) {
		cout << '-';
	}
	SetPos(0, 24);
	for (int i = 0; i <= 100; i++) {
		cout << '-';
	}
}

//player component
COORD position[4];
COORD centre;

//draw people according to the centre coordinate
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

//clear people on the screen
void draw_null_people() {
	for (int i = 0; i < 4; i++) {
		SetPos(position[i]);
		cout << ' ';
	}
}

//set start position of people
void initial_people() {
	centre.X = 1;
	centre.Y = 21;
	draw_people(centre);
}

//obstacle involves centre and different component coordinate
struct obstacle {
	COORD centre;
	COORD graphs[8];
};

//linked list to store obstacle
struct node {
	obstacle * this_obstacle;
	node * next;
};

//head_node and tail_node
node * head_node = new node;
node * tail_node = new node;

void set_head_node(node * &head_node) {
	head_node->this_obstacle = NULL;
	head_node->next = NULL;
}

void set_tail_node(node * &tail_node) {
	tail_node->this_obstacle = NULL;
	tail_node->next = NULL;
}

//add new node to linked list forward
void create_new_node(obstacle * this_obstacle, node * &head_node, node * &tail_node) {
	node * new_node = new node;
	new_node->this_obstacle = this_obstacle;
	new_node->next = NULL;
	if (head_node->this_obstacle == NULL) {
		head_node = new_node;
		tail_node = new_node;
	} else {
		tail_node->next = new_node;
		tail_node = new_node;
	}
}

//destroy node to realize dynamic memory
void destroy_node(node * &head_node) {
	if (head_node->next != NULL) {
		node * p = head_node;
		head_node = head_node->next;
		delete p;
	}
}

//generate obstacle and initial it
void initial_obstacle(obstacle * this_obstacle) {
	int height = (rand() % (0 - 2)) + 2;
	this_obstacle->centre = {100, 23 - height};
	this_obstacle->graphs[0].X = this_obstacle->graphs[2].X = this_obstacle->centre.X - 1;
	this_obstacle->graphs[1].X = this_obstacle->graphs[3].X = this_obstacle->centre.X + 1;
	this_obstacle->graphs[4].X = this_obstacle->centre.X - 2;
	this_obstacle->graphs[5].X = this_obstacle->centre.X + 2;
	this_obstacle->graphs[0].Y = this_obstacle->graphs[1].Y = this_obstacle->centre.Y - 1;
	this_obstacle->graphs[2].Y = this_obstacle->graphs[3].Y = this_obstacle->centre.Y + 1;
	this_obstacle->graphs[4].Y = this_obstacle->graphs[5].Y = this_obstacle->centre.Y;

}

//reset coordinate of obstacle
void reset_obstacle(obstacle * this_obstacle) {
	this_obstacle->graphs[0].X = this_obstacle->graphs[2].X = this_obstacle->centre.X - 1;
	this_obstacle->graphs[1].X = this_obstacle->graphs[3].X = this_obstacle->centre.X + 1;
	this_obstacle->graphs[4].X = this_obstacle->centre.X - 2;
	this_obstacle->graphs[5].X = this_obstacle->centre.X + 2;
	this_obstacle->graphs[0].Y = this_obstacle->graphs[1].Y = this_obstacle->centre.Y - 1;
	this_obstacle->graphs[2].Y = this_obstacle->graphs[3].Y = this_obstacle->centre.Y + 1;
	this_obstacle->graphs[4].Y = this_obstacle->graphs[5].Y = this_obstacle->centre.Y;

}

//draw obstacle
void draw_obstacle(obstacle * this_obstacle) {
	for (int i = 0; i < 4; i++) {
		SetPos(this_obstacle->graphs[i]);
		cout << '-';
	}
	for (int i = 4; i < 6; i++) {
		SetPos(this_obstacle->graphs[i]);
		cout << '|';
	}
}

//clear the obstacle on the screen
void draw_null_obstacle(obstacle * this_obstacle) {
	for (int i = 0; i < 6; i++) {
		SetPos(this_obstacle->graphs[i]);
		cout << ' ';
	}
}

//move all the obstacle and judge whether to destroy it or not
void obstacle_move() {
	node * current = head_node;
	while (current != NULL && current->this_obstacle != NULL) {
		//move one obstacle
		draw_null_obstacle(current->this_obstacle);
		current->this_obstacle->centre.X -= 2;
		reset_obstacle(current->this_obstacle);
		draw_obstacle(current->this_obstacle);
		//judgement
		if (current->this_obstacle->centre.X <= 1) {
			draw_null_obstacle(current->this_obstacle);
			destroy_node(head_node);
			current = head_node;
			continue;
		}
		current = current->next;
	}
	delete current;
}

//control the character to jump
void * jump(void * args) {
	for (int i = 0; i < 5; i++) {
		draw_null_people();
		centre.Y -= 1;
		draw_people(centre);
		sleep(100);
	}
	for (int i = 0; i < 5; i++) {
		draw_null_people();
		centre.Y += 1;
		draw_people(centre);
		sleep(100);
	}
}

void crash(obstacle * this_obstacle) {
	if (this_obstacle->centre.X - 2 <= centre.X + 1 && centre.Y + 1 <= this_obstacle->centre - 1) {
		game_over();
	}
} 

void game_over() {
	
}

void game() {
	obstacle * this_obstacle = new obstacle;
	initial_obstacle(this_obstacle);
	draw_obstacle(this_obstacle);
	create_new_node(this_obstacle, head_node, tail_node);
	while(true) {
		if (_kbhit()) {
		    char x = getchar();
		        if (x == 'k') {
				pthread_t tids;
				int ret = pthread_create(&tids, NULL, jump, NULL);
			}
		}
		if (Timer(5, 0)) {
			obstacle * this_obstacle = new obstacle;
			initial_obstacle(this_obstacle);
			draw_obstacle(this_obstacle);
			create_new_node(this_obstacle, head_node, tail_node);
		}
		if (Timer(1, 1)) {
			obstacle_move();
		}
	}
  pthread_exit(NULL);
}

int main() {
	srand((int)time(0));
	//	int new_win = system("gnome-terminal -e ./project");
	initial_people();
	draw_ground();
	set_head_node(head_node);
	set_tail_node(tail_node);
	
	game();
	return 0;
}
