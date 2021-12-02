//
// Created by Kevin Li on 2021/11/24.
//
#ifndef SRC_RANKLIST_H
#define SRC_RANKLIST_H
struct Node{
    int score;
    std::string id;
    Node *next;
    Node *previous;
};

class RankSortedList{
public:
    int size = 0;
    bool insert(int score, std::string id);
    void displayRank(int x, int y);
    void printAll();
    void exportList();
    void importList();
    void deleteAll();

private:
    char PATH[8] = "../Rank";
    Node *head;
    Node *tail;

};


#endif //SRC_RANKLIST_H