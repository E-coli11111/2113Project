#include <iostream>
#include <iomanip>
#include <conio.h>
#include <string>
#include <windows.h>
#include <time.h>
#include <pthread.h>

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

//set cursor position
void SetPos(COORD a) {
	HANDLE out=GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(out, a);
}

void SetPos(int i, int j) {
	COORD pos={i, j};
	SetPos(pos);
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
	obstacle * obstacle;
	node * next;
};

//head_node and tail_node
node * head_node = new node;
node * tail_node = new node;

void set_head_node(node * &head_node) {
	head_node->obstacle = NULL;
	head_node->next = NULL;
}

void set_tail_node(node * &tail_node) {
	tail_node->obstacle = NULL;
	tail_node->next = NULL;
}

//add new node to linked list forward
void create_new_node(obstacle * this_obstacle, node * &head_node, node * &tail_node) {
	node * new_node = new node;
	new_node->obstacle = this_obstacle;
	new_node->next = NULL;
	if (head_node->obstacle == NULL) {
		head_node = new_node;
		tail_node = new_node;
	} else {
		tail_node->next = new_node;
		tail_node = new_node;
	}
}

//destroy node to realize dynamic memory
void destroy_node(node * &head_node) {
	node * p = head_node;
	head_node = head_node->next;
	delete p;
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
	while (current != NULL) {
		//move one obstacle
		draw_null_obstacle(current->obstacle);
		current->obstacle->centre.X -= 2;
		reset_obstacle(current->obstacle);
		draw_obstacle(current->obstacle); 
		//judgement
		if (current->obstacle->centre.X <= 0) {
			draw_null_obstacle(current->obstacle);
			destroy_node(head_node);
			node * current = head_node;
			continue;
		}
		current = current->next;
	}
	delete current;	
}

//control the character to jump and judge destroy of the obstacle
void * jump(void * args) {
	for (int i = 0; i < 5; i++) {
		draw_null_people();
		centre.Y -= 1;
		draw_people(centre);
		Sleep(100);
	}
	for (int i = 0; i < 5; i++) {
/*		node * current = head_node;
		while(current != NULL) {
			//if people steps on the obstacle, destroy it and award score
			if ((current->obstacle->graphs[0].X == centre.X + 1 || current->obstacle->graphs[0].X == centre.X - 1 || current->obstacle->graphs[1].X == centre.X + 1 || current->obstacle->graphs[1].X == centre.X - 1) && (current->obstacle->graphs[1].Y == centre.Y + 2)) {
				draw_null_obstacle(current->obstacle);
				destroy_node(head_node);
				node * current = head_node;
				continue;
				//score++;
			}
			current = current->next;			
		}
		delete current; */
		draw_null_people();
		centre.Y += 1;
		draw_people(centre);
		Sleep(100);
	}
}

void game() {
	obstacle * this_obstacle = new obstacle;
	initial_obstacle(this_obstacle);
	draw_obstacle(this_obstacle);
	create_new_node(this_obstacle, head_node, tail_node);
	while(true) {
		if (_kbhit()) {
			char x = _getch();
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
	initial_people();
	draw_ground();
	set_head_node(head_node);
	set_tail_node(tail_node);
	game();
	return 0;
}
//jump判断脚下有无东西--判断光标处是否有输出
//异步操作进行跳跃和障碍移动
//设置链表储存障碍并随时更新 
