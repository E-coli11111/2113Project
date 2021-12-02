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
#include "Menu_Generator.h"
#include "Console_Operation.h"
#include "RankList.h"

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
timeval start[3];
timeval game_start;
timeval game_end;
int score = 0;
// player component
COORD position[4];
COORD centre;

// head_node and tail_node
node * head_node;
node * tail_node;

void restart_game();

// initial Timer by obtaining current time
void initial_Timer() {
  for (int i = 0; i < 3; i++) {
  	gettimeofday(&start[i], NULL);
  }
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
  for (int i = 0; i <= 76; i++) {
	cout << '-';
  }
  SetPos(0, 24);
  for (int i = 0; i <= 76; i++) {
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
	head_node = new node;
  head_node->this_obstacle = NULL;
  head_node->next = NULL;
}

void set_tail_node(node * &tail_node) {
	tail_node = new node;
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

// generate obstacle and initial it
void initial_obstacle(obstacle * this_obstacle) {
  int height = (rand() % (0 - 10)) + 10;
  this_obstacle->centre = {75, 23 - height};
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

// destroy node to realize dynamic memory
void destroy_node(node * &head_node) {
  if (head_node->next != NULL) {
	node * p = head_node;
	head_node = head_node->next;
	delete p;
  }
}

// save the score to the rank list
void save(int score, string name){
  RankSortedList list = RankSortedList();
  list.importList();
  list.insert(score, name);
  list.exportList();
  list.deleteAll();
}

// control the end of the game
void next_step() {
  SetPos(26,14);
  setColor(33);
  cout << "Press Space to go to the next step" << endl;
  while (true) {
    if (kbhit()) {
      char x = getchar();
      if (x == 32) {
        break;
      }
    }
  }
  save(score, name);
}

// output game over window
void game_over() {
  clear();
  SetPos(30,13);
  setColor(33);
  cout<< "You lose!!" << endl;
  gettimeofday(&game_end, NULL);
  score += 1000 * (game_end.tv_sec - game_start.tv_sec) + (game_end.tv_usec - game_start.tv_usec) / 1000;
  next_step();
  restart_game();
}

// judge whether player crashed on the obstacle
void crash() {
  node * current = head_node;
  while (current != NULL && current->this_obstacle != NULL) {
    if (current->this_obstacle->centre.X - 2 > centre.X + 1 || current->this_obstacle->centre.X + 2 < centre.X - 1 || centre.Y + 1 < current->this_obstacle->centre.Y - 1 || centre.Y - 1 > current->this_obstacle->centre.Y + 1) {
      current = current->next;
    } else {
      game_over();
    }
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
	if (current->this_obstacle->centre.X <= 3) {
	  draw_null_obstacle(current->this_obstacle);
	  destroy_node(head_node);
	  current = head_node;
	  continue;
	}
	current = current->next;
  }
  crash();
}

// control the character to up side
void up() {
  draw_null_people();
  centre.Y -= 1;
  draw_people(centre);
  crash();
}

// control the character to down side
void down() {
  draw_null_people();
  centre.Y += 1;
  draw_people(centre);
  crash();
}

// control the character to left side
void left() {
  draw_null_people();
  centre.X -= 1;
  draw_people(centre);
  crash();
}

// control the character to right side
void right() {
  draw_null_people();
  centre.X += 1;
  draw_people(centre);
  crash();
}

// stop the game
void Pause(){
  //codes to stop the game
  gettimeofday(&game_end, NULL);
  score += 1000 * (game_end.tv_sec - game_start.tv_sec) + (game_end.tv_usec - game_start.tv_usec) / 1000;
  bool quit = pauseMenu();
  if (quit) {
	clear();
    next_step();
    restart_game();
  } else {
    gettimeofday(&game_start, NULL);
  }
}

// play the game
void game() {
  obstacle * this_obstacle = new obstacle;
  initial_obstacle(this_obstacle);
  draw_obstacle(this_obstacle);
  create_new_node(this_obstacle, head_node, tail_node);
  int n = 0;
  while(true) {
	if (kbhit()) {
	  char x = getchar();
	  if (x == 'w' && centre.Y >= 5) {
	  	up(); 
	  } else if (x == 's' && centre.Y <= 20) {
	  	down();
	  } else if (x == 'a' && centre.X >= 2) {
	  	left();
	  } else if (x == 'd' && centre.X <= 74) {
	  	right();
	  } else if (x == 'p'){
        Pause();
	  }
    }
    if (Timer(5000, 0)) {
	  obstacle * this_obstacle = new obstacle;
	  initial_obstacle(this_obstacle);
	  draw_obstacle(this_obstacle);
	  create_new_node(this_obstacle, head_node, tail_node);
    }
    if (Timer(300 - 50 * n, 1)) {
	  obstacle_move();
    }
    if (Timer(20000, 2)) {
      n++;
	}
  }
}

int main() {
  mainMenu();
	clear();
  SetPos(30,13);
  setColor(33);
  cout << "Please enter your name(must end with space):";
  cin >> name;
  srand((int)time(0));
  initial_people();
  draw_ground();
  set_head_node(head_node);
  set_tail_node(tail_node);
  initial_Timer();
  gettimeofday(&game_start, NULL);
  game();
  
  return 0;
}

// used to free the whole linked list
void free_list(node * &head_node) {
    int i = 0;
	while (head_node != NULL) {
	    cout << i++ << endl;
        node * temp = head_node;
        head_node = head_node->next;
        free(temp);
    }
}

// restart the game after the last one is done
void restart_game() {
  clear();
  free_list(head_node);
  score = 0;
  mainMenu();
	clear();
  SetPos(30,13);
  setColor(33);
  cout << "Please enter your name(must end with space):";
  cin >> name;
  initial_people();
  draw_ground();
  set_head_node(head_node);
  set_tail_node(tail_node);
  initial_Timer();
  gettimeofday(&game_start, NULL);
  game();
  
}
