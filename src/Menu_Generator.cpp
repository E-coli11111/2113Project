#include <iostream>
#include <string>
#include "Console_Operation.h"
#include "RankList.h"

using namespace std;

char PATH[] = "Rank";//The path of the of the rank data.

void mainMenu();

/*
 * This method implement the ranking function.
 * It will first load the rank file from the local data.
 * And display it on the console.
 * Press "BACK" to return to the main menu.
 */
void rankMenu(){
    clear();
    setPos(33,10);
    setColor(33);
    cout<<"Top 5!!"<<endl;
    RankSortedList *listptr = new RankSortedList;
    listptr->importList(PATH);
    listptr->displayRank(25,12);
    setPos(34,17);
    setColor(31);
    cout << "BACK";
    setPos(33,17);
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

void pauseMenu(){
    setColor(35);
    setPos(33,0);
    cout << "RESUME";
    setPos(33,1);
    cout << "SAVE AND QUIT";
    setPos(32,0);
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
                //resume();
            }else if(lineNum == 1){
                //save();
                //quit();
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
    setPos(30,10);
    setColor(33);
    cout<<"WELCOME!!"<<std::endl;
    setColor(35);
    setPos(29,12);
    cout<<"START";
    setPos(29,13);
    cout<<"HIGHEST SCORES";
    setPos(29,14);
    cout<<"EXIT";
    setPos(28,12);
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
                exit(1);
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
int main(){
    mainMenu();
}

