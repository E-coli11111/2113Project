# 2113Project
## Team Number:
66  
## Team Members:  
- Wang Hongyu 3035772913  
- Li Zhuokai 3035772406  
## Game Name: **Flappy man**  
## Description  
This is a game where you are acting a kid who is running back home after school. You need to control the kid to get away of hitting different obstacles. The further you run, the higher points you will get.  
## Feature  
1. Controling the character  
2. Background moving  
3. Map generating (Requirement 1)  
    Maps and obstacles are generated randomly to improve the game experience  
4. Obstacle generating (Requirement 1)  
5. Point and the ranking list (Requirement 2)   
    We have implement a sorted list based on the linked list. When a player finished a game, the record will automatically calculated and saved.
6. Delete useless data (e.g. passed obstacles and maps) (Requirement 3)  
7. Saving and loading the ranking list (Requirement 4)  
    When the player want to read the records, the programe will read the local files. If there is a new record, changes will be immediately written into the local file
8. Separating tasks into different files (Requirement 5)  
    We have use 4 .cpp files and implement 3 headers.  
    
## How to compile  
Enter the project directory and use the following command in the console to conpile:  
`make`
Use `./main PLAYER_NAME` to activate the game
Use `make clean` to remove all the compiled file

## How to play  
USE WASD to control the character. Press P to pause the game.
When you are in the menu, use the W and S key to move the cursor and press Enter to go to the function.
When your character is being hit, you lose the game

## Notice
- Please make sure the console window is large enough. Otherwise the map may look strange.
- Please make sure the `Rank` file in this repo is in the game directory. **Not in `./src`.**  
- Please make sure the `Rank` file contains no empty line.
