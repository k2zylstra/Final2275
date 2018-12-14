//
// Created by kieran on 12/6/18.
//

#ifndef FINALPROJECT2275_GRAPH_H
#define FINALPROJECT2275_GRAPH_H

#include <vector>
#include <limits>
#include <climits>

using namespace std;

struct converge;

enum difficulty {
    blue, green, black, double_black, chair
};

struct run {

    difficulty dif;
    int length;
    int time;
    converge * node;
    int capacity;
    int flow;

    void calcTimeAndFlow() {

        time = length * (static_cast<int>(dif) + 1);
        flow = 0;
        capacity = (static_cast<int>(dif) + 1) * (length /100);

        if (capacity == 0) {
            capacity = 1;
        }
    }
};

struct converge {
/*
 * for the id's there are lots of converges that aren't the to of hills but
 *  the top of the hills are labelled as the following:
 * 0 is the bottom of the hill, 1 is Double Diamond, 2 is Tye Mill,
 * 3 is 7th Heaven, 4 is Big Chief, 5 is Daisy, 6 is HogsBack,
 * 7 is Skyline, 8 is Brooks
 */
    int id;
    vector<run*> runOptions;
    int distanceFromStart;
    int totalTime;
    converge *parent;
    bool visited;

    converge (int idToSet){
        id = idToSet;
        runOptions.clear();
        distanceFromStart = 0;
        parent = nullptr;
        visited = false;
        totalTime = INT_MAX;
    }
};

class Graph {
public:
    Graph();
    void insertConverge(int id);
    int addRun(int parentId, int endId, difficulty diff, int length);
    vector<converge*> getAdjList();
    vector<converge> getDistanceBetween(int start, int end);
    int getMaxFlow(int source, int sink);

private:
    vector<converge*> adjList;
    vector<converge> residualGraph;
    void copyAdjListToRes();
    bool bfs(int start, int end);
    converge *dijkstrasPath(converge* startC, int key);
    void resetValues();
    converge *searchVertex(int id);
    converge *searchVertexRes(int id);
    run * getOppisite(run * r, int parentId);
};


#endif //FINALPROJECT2275_GRAPH_H
