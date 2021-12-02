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
void moveDown(){
    cout << "\033[1B";
}

// Move the mouse to the column left
void moveLeft(int n){
    printf("\033[%dC", n);
}

// Move the mouse to the column right
void moveRight(int n){
    printf("\033[%dD", n);
}

// Clear everything in the console.
void clear(){
    for(int line = 0; line < 50 ; line ++){
        setPos(0,line);
        cout << "\033[K";
    }

}

// hide the position of the cursor
void hideCursor() {
    cout << "\033[?25l";
}

// show position of the cursor
void showCursor() {
    cout << "\33[?25h";
}

//Reference: https://blog.csdn.net/yemao_guyue/article/details/43105253
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
    system("stty -echo");
    int in;
    cout << "\033[s";
    while(1){
        if(kbhit()){
            in = getchar();

            break;
        }

    }
    system("stty echo");
    cout << "\033[u";
    return in;
}//这个是抄的到时得改
