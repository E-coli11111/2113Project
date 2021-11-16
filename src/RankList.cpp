#include <iostream>
#include <string>
#include <fstream>
const int MAXSIZE = 5;
struct node{
    int score;
    std::string id;
    node *next;
    node *previous;
};

/*
 * Data structure to store users score in order.
 * Based on linked list.
 * Return: True if user node is successfully added. Otherwise, return false.
 */
class RankSortedList{
public:
    node *head;
    node *tail;
    int size = 0;
    bool insert(int score, std::string id){
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
    void printAll(){
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
    void deleteAll(){
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
    void exportList(std::string path){
        std::ofstream fout;
        fout.open(path);
        if(fout.fail()) {
            std::cout << "Errors" << std::endl;
            return;
        }
        node *current = head;
        while(current != NULL){
            fout<< current->id << "\t" << current->score << std::endl;
            if(current->next == nullptr)
                break;
            current = current->next;
        }
    }
    // Import rank from existing file. Original data in the file will be release;
    void importList(std::string path){
        deleteAll();
        std::string data;
        std::ifstream fin;
        fin.open(path);
        if(fin.fail()){
            std::cout << "Errors" << std::endl;
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
private:

};