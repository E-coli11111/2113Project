#include <iostream>
#include <termio.h>
#include <stdio.h>

/*
 * Set the color of words printed in the console.
 * Input n should be with in [30,37]
 * 30: black        31: red
 * 32: green        33: yellow
 * 34: blue         35: purple
 * 36: dark green   37: white
 */
void setColor(int n){
    std::cout << "\033[" << to_string(n) + "m";
}

// Set the position of the console
void Setpos(int x, int y){
    printf("\033[%d;%dH", y, x); 
}
// Reset the font color and background color to default.
void resetColor(){
    std::cout << "\033[49;39m";
}

// Move the mouse to the line above.
void moveUp(){
    std::cout << "\033[1A";
}

// Move the mouse to the line below.
void moveDOWN(){
    std::cout << "\033[1B";
}

// Clear everything in the console.
void clear(){
    std::cout << "\033[2J";
}
/*
 * Detect if there is a keyboard activity.
 * Return the ASCII code of the key pressed on the keyboard.
 */
int scanKeyboard(){
    int in;
    struct termios new_settings;
    struct termios stored_settings;
    tcgetattr(0,&stored_settings);
    new_settings = stored_settings;
    new_settings.c_lflag &= (~ICANON);
    new_settings.c_cc[VTIME] = 0;
    tcgetattr(0,&stored_settings);
    new_settings.c_cc[VMIN] = 1;
    tcsetattr(0,TCSANOW,&new_settings);

    in = getchar();

    tcsetattr(0,TCSANOW,&stored_settings);
    return in;
}//这个是抄的到时得改

