#include <iostream>
#include "Console_Operation.h"

/*
 * Generate the starting menu.
 * Press "START" to start the game.
 * Press "HIGHEST SCORES" to see the top 5 highest score.
 * More functions can be added.
 */
void mainMenu(){ //May need to set the initial mouse to the right position.
    setPos(30,10);
    setColor(33);
    std::cout<<"WELCOME!!"<<std::endl;
    setColor(35);
    setPos(29,12);
    std::cout<<"START";
    setPos(29,13);
    std::cout<<"HIGHEST SCORES";
    setPos(28,12);
    setPos(28,12);
    int lineNum = 0; //If lineNum = 0, start the game. If lineNum = 1, show the highest score
    while(1){
        int key = scanKeyboard();
        if(key == 119){ // When press "W", go up.
            if(lineNum == 1){
                lineNum --;
                setPos(28,12);
            }
        }
        if(key == 115){ // When press "S", go down.
            if(lineNum == 0){
                lineNum ++;
                setPos(28,13);
            }
        }
        if(key == 10){ // When press "Enter", go to the selected function.
            if(lineNum == 0)
                exit(1);
            else if(lineNum == 1)
                  exit(2);
            break;
        }

    }
    clear();
}

