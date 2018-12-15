//
// Created by kieran on 12/6/18.
//

#include <iostream>
#include <queue>
#include "Graph.h"


using namespace std;

struct comparator {

        bool operator() (converge *i, converge *j) {
            return i->totalTime > j->totalTime;
        }
};

Graph::Graph() {
    //TODO turn the adjList into a hash
    //adjList.assign(27, 0);
}


void Graph::insertConverge(int id) {

    converge *c = new converge(id);
    adjList.push_back(c);
}

int Graph::addRun(int parentId, int endId, difficulty diff, int length) {

    int returnVal = 0;
    converge *p = searchVertex(parentId);
    converge *e = searchVertex(endId);

    if (p || e == nullptr) { //TODO may have to edit this

        run *r = new run();
        r->dif = diff;
        r->length = length;
        r->node = e;
        r->calcTimeAndFlow();
        p->runOptions.push_back(r);
    }
    else {
        cout << "One or more of the converges with the specified id could not be found" << endl;
        returnVal = -1;
    }
    return returnVal;
}

vector<converge*> Graph::getAdjList() {

    return adjList;
}

vector<converge> Graph::getDistanceBetween(int start, int end) {

    vector<converge> toReturn;
    converge *startC = searchVertex(start);
    converge *endC = searchVertex(end);

    if (startC != nullptr && endC != nullptr) {

        converge *endConverge = dijkstrasPath(startC, end);

        if (endConverge != nullptr) {

            toReturn.push_back(*endConverge);

            while (endConverge->parent != nullptr) {
                toReturn.push_back(*(endConverge->parent));
                endConverge = endConverge->parent;
            }
        }
    }
    else {
        cout << "One or more of the given vertex names did not exist in the graph" << endl;
    }

    return toReturn;
}

converge* Graph::dijkstrasPath(converge* startC, int key) {

    resetValues();
    priority_queue<converge*, vector<converge*>, comparator> minHeap;
    minHeap.push(startC);
    startC->totalTime = 0;

    while(!minHeap.empty()) {

        converge *tmp = minHeap.top();
        minHeap.pop();

        if (tmp->id == key) {
            return tmp;
        }

        for (int i = 0; i < tmp->runOptions.size(); i++) {

            int t = tmp->totalTime + tmp->runOptions[i]->time;

            if (t < tmp->runOptions[i]->node->totalTime) {

                tmp->runOptions[i]->node->parent = tmp;
                tmp->runOptions[i]->node->totalTime = t;
                minHeap.push(tmp->runOptions[i]->node);
            }
        }
    }

    return nullptr;
}

int Graph::getMaxFlow(int source, int sink) {

    resetValues();
    int toReturn = 0;
    copyAdjListToRes();
    converge* sourceC = searchVertexRes(source);
    converge* sinkC = searchVertexRes(sink);

    if (sourceC && sinkC != nullptr) {


        while(bfs(source, sink)) {

            converge *traceC = sinkC;
            converge *childC;
            run *edge;

            int pathMinFlow = INT_MAX;
            while (traceC->parent != nullptr) {

                childC = traceC;
                traceC = traceC->parent;


                for (int i = 0; i < traceC->runOptions.size(); i++) {

                    edge = traceC->runOptions[i];
                    if (traceC->runOptions[i]->node == childC) {
                        break;
                    }
                }
                pathMinFlow = min(pathMinFlow, edge->capacity - edge->flow);
            }

            traceC = sinkC;
            while(traceC->parent != nullptr) {

                childC = traceC;
                traceC = traceC->parent;

                for (int i = 0; i < traceC->runOptions.size(); i++) {

                    edge = traceC->runOptions[i];
                    if (traceC->runOptions[i]->node == childC) {
                        break;
                    }
                }

                edge->flow += pathMinFlow;

                edge = getOppisite(edge, traceC->id);
                edge->flow -= pathMinFlow;

            }

            toReturn += pathMinFlow;
        }

        residualGraph.clear();
    }
    else {
        cout << "One or more of the specified nodes could not be found" << endl;
        toReturn = -1;
    }

    return toReturn;
}

bool Graph::bfs(int start, int end) {

    for (int i = 0; i < residualGraph.size(); i++) {
        residualGraph[i].parent = nullptr;
        residualGraph[i].visited = false;
    }
    converge * startC = searchVertexRes(start);
    converge * endC = searchVertexRes(end);

    queue<converge*> Q;
    Q.push(startC);
    startC->visited = true;

    while(!Q.empty()) {

        converge *tmp;
        tmp = Q.front();
        Q.pop();

        for (int i = 0; i < tmp->runOptions.size(); i++) {

            if (!tmp->runOptions[i]->node->visited && tmp->runOptions[i]->flow < tmp->runOptions[i]->capacity) {

                Q.push(tmp->runOptions[i]->node);
                tmp->runOptions[i]->node->visited = true;
                tmp->runOptions[i]->node->parent = tmp;
            }
        }
    }

    return (endC->visited);
}

void Graph::copyAdjListToRes() {


//    converge *source = new converge(0);
//    converge *c1 = new converge(1);
//    converge *c2 = new converge(2);
//    converge *c3 = new converge(3);
//    converge *sink = new converge(4);
//
//    residualGraph.push_back(*source);
//    residualGraph.push_back(*c1);
//    residualGraph.push_back(*c2);
//    residualGraph.push_back(*c3);
//    residualGraph.push_back(*sink);
//
//    run *r01 = new run();
//    r01->node = &residualGraph[1];
//    r01->capacity =8;
//    r01->flow = 0;
//
//    run *r10 = new run();
//    r10->node = &residualGraph[0];
//    r10->capacity =8;
//    r10->flow = 8;
//
//    run *r14 = new run();
//    r14->node = &residualGraph[4];
//    r14->capacity =11;
//    r14->flow = 0;
//
//    run *r41 = new run();
//    r41->node = &residualGraph[1];
//    r41->capacity =11;
//    r41->flow = 11;
//
//    run *r13 = new run();
//    r13->node = &residualGraph[3];
//    r13->capacity =4;
//    r13->flow = 0;
//
//    run *r31 = new run();
//    r31->node = &residualGraph[1];
//    r31->capacity =4;
//    r31->flow = 4;
//
//    run *r02 = new run();
//    r02->node = &residualGraph[2];
//    r02->capacity =5;
//    r02->flow = 0;
//
//    run *r20 = new run();
//    r20->node = &residualGraph[0];
//    r20->capacity =5;
//    r20->flow = 5;
//
//    run *r23 = new run();
//    r23->node = &residualGraph[3];
//    r23->capacity =10;
//    r23->flow = 0;
//
//    run *r32 = new run();
//    r32->node = &residualGraph[2];
//    r32->capacity =10;
//    r32->flow = 10;
//
//    run *r34 = new run();
//    r34->node = &residualGraph[4];
//    r34->capacity =12;
//    r34->flow = 0;
//
//    run *r43 = new run();
//    r43->node = &residualGraph[3];
//    r43->capacity =12;
//    r43->flow = 0;
//
//
//
//    residualGraph[0].runOptions.push_back(r01);
//    residualGraph[0].runOptions.push_back(r02);
//    residualGraph[1].runOptions.push_back(r14);
//    residualGraph[1].runOptions.push_back(r13);
//    residualGraph[1].runOptions.push_back(r10);
//    residualGraph[2].runOptions.push_back(r23);
//    residualGraph[2].runOptions.push_back(r20);
//    residualGraph[3].runOptions.push_back(r34);
//    residualGraph[3].runOptions.push_back(r31);
//    residualGraph[3].runOptions.push_back(r32);
//    residualGraph[4].runOptions.push_back(r41);
//    residualGraph[4].runOptions.push_back(r43);

    //delete previous residual graph
    for (int i = 0; i < residualGraph.size(); i++) {

        for (int j = 0; j < residualGraph[i].runOptions.size(); j++) {

            delete residualGraph[i].runOptions[j];
        }
    }
    residualGraph.clear();

    for (int i = 0; i < adjList.size(); i++) {

        residualGraph.push_back(*adjList[i]);
        residualGraph[i].runOptions.clear();
    }

    for (int i =0; i < adjList.size(); i++) {

        for (int j = 0; j < adjList[i]->runOptions.size(); j++) {

            run * r = new run();
            r->node = searchVertexRes(adjList[i]->runOptions[j]->node->id);
            r->dif = adjList[i]->runOptions[j]->dif;
            r->capacity = adjList[i]->runOptions[j]->capacity;
            r->length = adjList[i]->runOptions[j]->length;
            r->flow = adjList[i]->runOptions[j]->flow;
            r->time = adjList[i]->runOptions[j]->time;

            residualGraph[i].runOptions.push_back(r);
        }
    }

    for (int i = 0; i < residualGraph.size(); i++) {

        for (int j = 0; j < residualGraph[i].runOptions.size(); j++) {

            for (int k = j + 1; k < residualGraph[i].runOptions.size(); k++) {

                if (residualGraph[i].runOptions[j]->node->id == residualGraph[i].runOptions[k]->node->id) {

                    run *r = new run();
                    r->capacity = residualGraph[i].runOptions[j]->capacity + residualGraph[i].runOptions[k]->capacity;

                    residualGraph[i].runOptions[j] = r;

                    residualGraph[i].runOptions.erase(residualGraph[i].runOptions.begin() + k - 1);
                    --k;
                }
            }
        }
    }

    for (int i = 0; i < adjList.size(); i++) {

        for (int j = 0; j < residualGraph[i].runOptions.size(); j++) {

            converge *front = searchVertexRes(residualGraph[i].runOptions[j]->node->id);

            bool contains = false;
            for (int k = 0; k < front->runOptions.size(); k++) {

                if (front->runOptions[k]->node->id == residualGraph[i].id) {
                    contains = true;
                    break;
                }
            }

            if (!contains) {
                run *r = new run();
                r->capacity = residualGraph[i].runOptions[j]->capacity;
                r->flow = r->capacity;
                r->node = &residualGraph[i];

                front->runOptions.push_back(r);
            }
        }
    }
}

run* Graph::getOppisite(run *r, int parentId) {

    run *toReturn;

    for (int i = 0; i < r->node->runOptions.size(); i++) {

        if (r->node->runOptions[i]->node->id == parentId) {
            toReturn = r->node->runOptions[i];
            break;
        }
    }

    return toReturn;
}


void Graph::resetValues() {

    for (int i = 0; i < adjList.size(); i++) {

        adjList[i]->visited = false;
        adjList[i]->parent = nullptr;
        adjList[i]->totalTime = INT_MAX;
    }
}

converge* Graph::searchVertex(int id) {

    for (int i = 0; i < adjList.size(); i++) {

        if (id == adjList[i]->id) {
            return adjList[i];
        }
    }
    return nullptr;
}

converge* Graph::searchVertexRes(int id) {

    for (int i = 0; i < residualGraph.size(); i++) {

        if (id == residualGraph[i].id) {
            return &residualGraph[i];
        }
    }

    return nullptr;
}
