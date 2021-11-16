#include <iostream>
#include "Console_Operation.h"

/*
 * Generate the starting menu.
 * Press "START" to start the game.
 * Press "HIGHEST SCORES" to see the top 5 highest score.
 * More functions can be added.
 */
void mainMenu(){ //May need to set the initial mouse to the right position.
    setColor(33);
    std::cout<<"WELCOME!!"<<endl;
    setColor(35);
    std::cout<<"START"<<endl<<"HIGHEST SCORES";
    int lineNum = 0; //If lineNum = 0, start the game. If lineNum = 1, show the highest score
    while(1){
        char key = scanKeyboard();
        if(key == 87){ // When press "W", go up.
            if(lineNum == 1){
                lineNum --;
                moveUp();
            }
        }
        if(key == 83){ // When press "S", go down.
            if(lineNum == 0){
                lineNum ++;
                moveUp();
            }
        }
        if(key == 13){ // When press "Enter", go to the selected function.
            if(lineNum == 0)
                startGame();
            else if(lineNum == 1)
                showRank();
            break;
        }
    clear();
    }
}