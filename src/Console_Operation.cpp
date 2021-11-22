#include <iostream>
#include <termio.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "string"
using namespace std;
/*
 * Set the color of words printed in the console.
 * Input n should be with in [30,37]
 * 30: black        31: red
 * 32: green        33: yellow
 * 34: blue         35: purple
 * 36: dark green   37: white
 */
void setColor(int n){
    cout << "\033[" << to_string(n) + "m";
}

// Set the position of the console
void setPos(int x, int y){
    printf("\033[%d;%dH", y, x); 
}
// Reset the font color and background color to default.
void resetColor(){
    cout << "\033[49;39m";
}

// Move the mouse to the line above.
void moveUp(){
    cout << "\033[1A";
}

// Move the mouse to the line below.
void moveDOWN(){
    cout << "\033[1B";
}

// Clear everything in the console.
void clear(){
    cout << "\033[2J";
}
/*
 * Detect keyboard activity
 * If detected, return 1, otherwise return 0.
 */
int kbhit(void){
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
    if(ch != EOF){
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

/*
 * Detect if there is a keyboard activity.
 * Return the ASCII code of the key pressed on the keyboard.
 */
int scanKeyboard(){
    system('stty -echo");
    int in;
    while(1){
        if(kbhit()){
            in = getchar();
            break;
        }

    }
    system('stty echo");
    return in;
}

