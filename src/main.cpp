#include <iostream>
#include <iomanip>
#include <pthread.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
//#include "Menu_Generator.h"
//#include "Console_Operation.h"

using namespace std;

// COORD is to store the coordinate of a point
struct COORD{
  int X;
  int Y;
};

// obstacle involves centre and different component coordinate
struct obstacle {
  COORD centre;
  COORD graphs[8];
};

// linked list to store obstacle
struct node {
  obstacle * this_obstacle;
  node * next;
};

// record time
timeval start[2];
timeval game_start;
timeval game_end;

// player component
COORD position[4];
COORD centre;

// head_node and tail_node
node * head_node = new node;
node * tail_node = new node;

// initial Timer by obtaining current time
void initial_Timer() {
  gettimeofday(&start[0], NULL);
  gettimeofday(&start[1], NULL);
}

// a timing machine
bool Timer(time_t time_period, int id) {
  timeval end;
  gettimeofday(&end, NULL);
  if (1000*(end.tv_sec - start[id].tv_sec) + (end.tv_usec - start[id].tv_usec)/1000 >= time_period) {
	start[id] = end;
	return 1;
  }
  return 0;
}

// read input from keyboard without stop the program
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

// set cursor position based on coordinate
void SetPos(int x, int y){
  printf("\033[%d;%dH", y, x);
}

// set cursor position based on COORD
void SetPos(COORD a) {
  SetPos(a.X, a.Y);
}

// move cursor's position
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
  
// draw the ground
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

// draw people according to the centre coordinate
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
  cout << 'o';
  SetPos(position[1]);
  cout << '|';
  SetPos(position[2]);
  cout << '/';
  SetPos(position[3]);
  cout << '\\';
}

// clear people on the screen
void draw_null_people() {
  for (int i = 0; i < 4; i++) {
	SetPos(position[i]);
	cout << ' ';
  }
}

// set start position of people
void initial_people() {
  centre.X = 1;
  centre.Y = 21;
  draw_people(centre);
}

// initialize head_node and tail_node
void set_head_node(node * &head_node) {
  head_node->this_obstacle = NULL;
  head_node->next = NULL;
}

void set_tail_node(node * &tail_node) {
  tail_node->this_obstacle = NULL;
  tail_node->next = NULL;
}

// add new node to linked list forwardly
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

// destroy node to realize dynamic memory
void destroy_node(node * &head_node) {
  if (head_node->next != NULL) {
	node * p = head_node;
	head_node = head_node->next;
	delete p;
  }
}

// generate obstacle and initial it
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

// reset coordinate of obstacle
void reset_obstacle(obstacle * this_obstacle) {
  this_obstacle->graphs[0].X = this_obstacle->graphs[2].X = this_obstacle->centre.X - 1;
  this_obstacle->graphs[1].X = this_obstacle->graphs[3].X = this_obstacle->centre.X + 1;
  this_obstacle->graphs[4].X = this_obstacle->centre.X - 2;
  this_obstacle->graphs[5].X = this_obstacle->centre.X + 2;
  this_obstacle->graphs[0].Y = this_obstacle->graphs[1].Y = this_obstacle->centre.Y - 1;
  this_obstacle->graphs[2].Y = this_obstacle->graphs[3].Y = this_obstacle->centre.Y + 1;
  this_obstacle->graphs[4].Y = this_obstacle->graphs[5].Y = this_obstacle->centre.Y;

}

// draw obstacle
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

// clear the obstacle on the screen
void draw_null_obstacle(obstacle * this_obstacle) {
  for (int i = 0; i < 6; i++) {
	SetPos(this_obstacle->graphs[i]);
	cout << ' ';
  }
}

// output game over window
void game_over() {
  //clear();
  SetPos(30,10);
  //setColor(33);
  cout<< "You lose!!" << endl;
  gettimeofday(&game_end, NULL);
  int score = 1000 * (game_end.tv_sec - game_start.tv_sec) + (game_end.tv_usec - game_start.tv_usec) / 1000;
  //mainMenu();
}

// judge whether player crashed on the obstacle
void crash(obstacle * this_obstacle) {
  if (this_obstacle->centre.X - 2 <= centre.X + 1 && centre.Y + 1 <= this_obstacle->centre.Y - 1) {
	game_over();
  }
} 

// move all the obstacle and judge whether to destroy it or not
void obstacle_move() {
  node * current = head_node;
  while (current != NULL && current->this_obstacle != NULL) {
	// move one obstacle
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
  crash(head_node->this_obstacle);
  delete current;
}

// control the character to jump
void * jump(void * args) {
  for (int i = 0; i < 5; i++) {
	draw_null_people();
	centre.Y -= 1;
	draw_people(centre);
	crash(head_node->this_obstacle);
	sleep(100);
  }
  for (int i = 0; i < 5; i++) {
	draw_null_people();
	centre.Y += 1;
	draw_people(centre);
	crash(head_node->this_obstacle);
	sleep(100);
  }
}

// play the game
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
    if (Timer(5000, 0)) {
	  obstacle * this_obstacle = new obstacle;
	  initial_obstacle(this_obstacle);
	  draw_obstacle(this_obstacle);
	  create_new_node(this_obstacle, head_node, tail_node);
    }
    if (Timer(1000, 1)) {
	  obstacle_move();
    }
  }
  pthread_exit(NULL);
}

int main() {
  //mainMenu();
  srand((int)time(0));
  //	int new_win = system("gnome-terminal -e ./project");
  initial_people();
  draw_ground();
  set_head_node(head_node);
  set_tail_node(tail_node);
  initial_Timer();
  gettimeofday(&game_start, NULL);
  game();
  
  return 0;
}
