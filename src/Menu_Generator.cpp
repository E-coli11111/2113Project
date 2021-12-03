#include <iostream>
#include <string>
#include "Console_Operation.h"
#include "RankList.h"

using namespace std;

void mainMenu();
void game();

/*
 * This method implement the ranking function.
 * It will first load the rank file from the local data.
 * And display it on the console.
 * Press "BACK" to return to the main menu.
 */
void rankMenu(){
    clear();
    setPos(35,8);
    setColor(33);
    cout<<"Top 5!!"<<endl;
    RankSortedList *listptr = new RankSortedList;
    listptr->importList();
    listptr->displayRank(28,10);
    setPos(36,17);
    setColor(31);
    cout << "BACK";
    setPos(36,17);
    while(1) {
        int key = scanKeyboard();
        if (key == 10) { // When press "Enter", back to main menu.
            clear();
            break;
        }
    }
    delete listptr;
    mainMenu();
}
/*
 * The menu generated when the game is paused.
 * Return true if the user wants to quit the game.
 * Return false if the user wants to resume the game.
 */
bool pauseMenu(){
    setColor(35);
    setPos(33,1);
    cout << "RESUME";
    setPos(33,2);
    cout << "SAVE AND QUIT";
    setPos(32,1);
    int lineNum = 0;
    while(1){
        int key = scanKeyboard();
        if(key == 119){ // When press "W", go up.
            if(lineNum != 0){
                lineNum --;
                moveUp();
            }
        }
        if(key == 115){ // When press "S", go down.
            if(lineNum != 1){
                lineNum ++;
                moveDown();
            }
        }
        if(key == 10){ // When press "Enter", go to the selected function.
            if(lineNum == 0) {
                setPos(1,1);
                cout << "\033[K";
                setPos(1,2);
                cout << "\033[K";
                return false;
            }else if(lineNum == 1){
                setPos(1,1);
                cout << "\033[K";
                setPos(1,2);
                cout << "\033[K";
                return true;
            }
            break;
        }
    }
}

/*
 * Generate the starting menu.
 * Press "START" to start the game.
 * Press "HIGHEST SCORES" to see the top 5 highest score.
 * More functions can be added.
 */
void mainMenu(){ //May need to set the initial mouse to the right position.
    clear();
    setPos(33,8);
    setColor(33);
    cout<<"WELCOME!!"<<std::endl;
    setColor(35);
    setPos(32,10);
    cout<<"START";
    setPos(32,11);
    cout<<"HIGHEST SCORES";
    setPos(32,12);
    cout<<"EXIT";
    setPos(31,10);
    int lineNum = 0; //If lineNum = 0, start the game. If lineNum = 1, show the highest score
    while(1){
        int key = scanKeyboard();
        if(key == 119){ // When press "W", go up.
            if(lineNum != 0){
                lineNum --;
                moveUp();
            }
        }
        if(key == 115){ // When press "S", go down.
            if(lineNum != 2){
                lineNum ++;
                moveDown();
            }
        }
        if(key == 10){ // When press "Enter", go to the selected function.
            if(lineNum == 0)
                break;
            else if(lineNum == 1)
                rankMenu();
            else if(lineNum == 2){
                clear();
                setPos(0,0);
                exit(0);
            }
            break;
        }
    }
    clear();
}