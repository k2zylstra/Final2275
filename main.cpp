#include <iostream>
#include <fstream>
#include <sstream>

#include "Graph.h"

using namespace std;

struct runStore {

    int endId;
    int parentId;
    difficulty diff;
    int length;
};

void outVector(vector<converge> v) {

    for (int i = v.size() - 1; i >= 0; i--) {
        cout << v[i].id << ",";
    }
    cout << endl << "===============" << endl;
}

difficulty convertStringToEnum(string str) {

    difficulty diff;

    if (str == "green") {
        diff = green;
    }
    else if (str == "blue") {
        diff = blue;
    }
    else if (str == "black") {
        diff = black;
    }
    else if (str == "double_black") {
        diff = double_black;
    }
    else if (str == "chair") {
        diff = chair;
    }

    return diff;
}

void testAddConvergeRun() {
    Graph *g = new Graph();
    g->insertConverge(0);
    g->insertConverge(1);
    g->addRun(0, 1, black, 750);
}

void testDijkstras(Graph *g) {
    outVector(g->getDistanceBetween(0, 12));
    outVector(g->getDistanceBetween(0, 2));
    outVector(g->getDistanceBetween(0, 4));
    outVector(g->getDistanceBetween(0, 10));
}

void testMaxFlow(Graph *g) {
    cout << "From 3 to 0" << endl;
    cout << g->getMaxFlow(3, 0) << endl;
    cout << "From 0 to 3" << endl;
    cout << g->getMaxFlow(0, 3) << endl;
}

void buildGraph(Graph *g, string filePath) {

    ifstream dataFile(filePath);
    vector<runStore*> runs;

    if (dataFile.is_open()) {

        string line;
        while(getline(dataFile, line)) {

            stringstream ss;
            ss << line;

            string idStr;
            ss >> idStr;

            g->insertConverge(stoi(idStr));

            string subLine;
            while(getline(ss, subLine, ',')) {

                stringstream ss1;
                ss1 << subLine;

                string endIdStr;
                getline(ss1, endIdStr, '-');

                string diffStr;
                getline(ss1, diffStr, '-');

                string lengthStr;
                getline(ss1, lengthStr, '-');

               runStore *rs = new runStore();
               rs->parentId = stoi(idStr);
               rs->endId = stoi(endIdStr);
               rs->diff = convertStringToEnum(diffStr);
               rs->length = stoi(lengthStr);
               runs.push_back(rs);
            }
        }

        for (int i = 0; i < runs.size(); i++) {

            g->addRun(runs[i]->parentId, runs[i]->endId, runs[i]->diff, runs[i]->length);
        }

        dataFile.close();
    }
    else {
        cout << "was unable to open the file" << endl;
    }
}

void getMaxFlow(Graph *g) {

    cout << "Enter a source node" << endl;
    string str1;
    getline(cin, str1);
    cout << "Enter a sink node" << endl;
    string str2;
    getline(cin, str2);

    cout << g->getMaxFlow(stoi(str1), stoi(str2)) << endl;
}

int main() {

    try {
        testAddConvergeRun();
    }
    catch(...) {
        cout << "test 1 failed" << endl;
    }

    Graph *g = new Graph();
    string filePath = "";

    //buildGraph(g, "/home/kieran/Documents/CSCI2275/FinalProject2275/stevens_pass_nodes");
    //testMaxFlow(g);
    bool isContinue = true;
    while(isContinue) {

        cout << "=== Menu ===" << endl;
        cout << "1. Build Graph" << endl;
        cout << "2. Get Max Flow" << endl;
        cout << "3. Get shortest distance" << endl;
        cout << "4. Quit" << endl;
        cout << "5. Test Dijkstras" << endl;
        cout << "6. Test max flow" << endl;

        string choiceStr;
        getline(cin, choiceStr);
        int choice = stoi(choiceStr);

        switch(choice) {
            case 1: {
                cout << "Enter in the file path or press enter to use default:" << endl;
                getline(cin, filePath);
                if (filePath == "") {
                    filePath = "/home/kieran/Documents/CSCI2275/FinalProject2275/stevens_pass_nodes";
                }
                buildGraph(g, filePath);
            }
                break;
            case 2:
                getMaxFlow(g);
                break;
            case 3:
                break;
            case 4:
                isContinue = false;
                break;
            case 5:
                testDijkstras(g);
                break;
            case 6:
                testMaxFlow(g);
                break;
        }
    }

    return 0;
}