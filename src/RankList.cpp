#include <iostream>
#include <string>
#include <fstream>
#include "Console_Operation.h"
#include "RankList.h"

using namespace std;

const int MAXSIZE = 5; // Can be changed.

/*
 * Data structure to store users score in order.
 * Based on linked list.
 * Return: True if user node is successfully added. Otherwise, return false.
 */

// Insert user's id and score to the List.
bool RankSortedList::insert(int score, string id){
    if(size == MAXSIZE && score <= tail->score){ //If the score is smaller than all the scores exist, it will not be added.
        return false;
    }
    node *toAdd = new node{score,id};
    node *current = head;
    if (size == 0 || score >= head->score){
        toAdd->next = head; // When the node is added to the first, update the head.
        if(size != 0)
            head->previous = toAdd;
        head = toAdd;
        if (size == 0)
            tail = toAdd;
    }else if(size < MAXSIZE) {
        toAdd->previous = tail;
        tail->next = toAdd;
        tail = toAdd;
    }else{
        while(current!=NULL) {
            current = current->next;
            if (score > current->score) {
                toAdd->previous = current->previous;
                toAdd->next = current;
                current->previous->next = toAdd;
                current->previous = toAdd;
                break;
            }
        }
    }
    size ++;
    if(size > MAXSIZE) {
        tail->previous->next = NULL;
        tail = tail->previous;
        size --;
    }
    return true;
}
void RankSortedList::displayRank(int x, int y){
    setColor(34);
    setPos(x,y);
    if(head == nullptr || head == NULL){
        cout<< "No record" << endl;
        return;
    }
    node *current = head;
    int rank = 1;
    while(current != nullptr){
        cout<< "#" << rank++ << "\t" <<current->id << "\t" << current->score << endl;
        if(current->next == nullptr)
            break;
        current = current->next;
        setPos(x,++y);
    }
}

// Print all the ids and scores in the list. Arranged by score.
void RankSortedList::printAll(){
    if(head == nullptr || head == NULL){
        std::cout<< "Empty List" << std::endl;
        return;
    }
    node *current = head;
    while(current != nullptr){
        std::cout<< current->id << "\t" << current->score << std::endl;
        if(current->next == nullptr)
            break;
        current = current->next;
    }
}
// Export data to the local path without any encryption. Can be modified easily.
void RankSortedList::exportList(char path[]){
    ofstream fout;
    fout.open(path);
    if(fout.fail()) {
        std::cout << "Errors" << std::endl;
        return;
    }
    node *current = head;
    while(current != NULL){
        fout<< current->id << "\t" << current->score << endl;
        if(current->next == nullptr)
            break;
        current = current->next;
    }
}
// Import rank from existing file. Original data in the file will be release;
void RankSortedList::importList(char path[]){
    deleteAll();
    std::string data;
    std::ifstream fin;
    fin.open(path);
    if(fin.fail()){
        std::cout << "Error when loading ranks!" << std::endl;
    }
    int pos, score;
    std::string id;
    if(std::getline(fin,data)){
        pos = data.find("\t");
        id = data.substr(0, pos);
        score =std::stoi(data.substr(pos + 1, data.length() - pos - 1));
        node *ele = new node{score,id};
        head = ele;
    }
    node *current = head;
    while(std::getline(fin, data)){
        pos = data.find("\t");
        id = data.substr(0, pos);
        score =std::stoi(data.substr(pos + 1, data.length() - pos - 1));
        node *ele = new node{score,id};
        current->next = ele;
        ele->previous = current;
        current = ele;
    }
    tail = current;
    fin.close();
}

void RankSortedList::deleteAll(){
    if(size == 0){
        return;
    }else if(size == 1){
        delete head;
    }else{
        node *current = tail->previous;
        node *temp;
        delete tail;
        tail = NULL;
        while(current != head){
            temp = current;
            current = temp->previous;
            delete temp;
            temp = NULL;
        }
        delete head;
        head = NULL;
    }
    size = 0;
}


