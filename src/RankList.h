//
// Created by Kevin Li on 2021/11/24.
//
#ifndef SRC_RANKLIST_H
#define SRC_RANKLIST_H
struct node{
    int score;
    std::string id;
    node *next;
    node *previous;
};

class RankSortedList{
public:
    int size = 0;
    bool insert(int score, std::string id);
    void displayRank(int x, int y);
    void printAll();
    void exportList(char path[]);
    void importList(char path[]);

private:
    node *head;
    node *tail;
    void deleteAll();
};


#endif //SRC_RANKLIST_H