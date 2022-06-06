//
// Created by jpdat on 04-06-2022.
//

#ifndef PROJ2_MENU_H
#define PROJ2_MENU_H

#include <map>
#include <utility>
#include <vector>
#include "Graph.h"

class Menu {
public:
    Graph<int> graph;
    int origin = 0;
    int target = 0;
    static int querryEnd() {
        cout << "What to do now?\n"
                "1 - Return\n"
                "0 - Exit\n";
        return intInput(0, 1);
    }

    static int intInput(int min, int max) {
        string input;
        int output;
        do {
            cin >> input;
            if (input == "-1") {
                return -1;
            }
            try {
                output = stoi(input);
            } catch (invalid_argument &e) {
                cout << "I can't understand your input, could you please try again?\n";
                output = -2;
            }
            if (output < min || output > max) {
                cout << "I can't understand your input, could you please try again?\n";
                output = -2;
            }
        } while (output == -2);
        return output;
    }

    void runSelectOrigin() {
        cout << "Welcome to the Path Finder! Your algorithmic interface to find best paths through"
                " Graph data structures :)\n"
                "Now, where are we starting? (Input the integer representing the node)\n";
        bool repeat;
        do {
            repeat = false;
            origin = intInput(1, INT32_MAX);
            if (graph.findVertex(origin) == nullptr) {
                cout << "Sorry, couldn't find that vertex. Can you try again?\n";
                repeat = true;
            }
        } while (repeat);
        runSelectTarget();
    }

    void runSelectTarget() {
        cout << "Great, now let's select our target destination. (Input the integer representing the node)\n";
        bool repeat;
        do {
            repeat = false;
            target = intInput(1, INT32_MAX);
            if (graph.findVertex(target) == nullptr) {
                cout << "Sorry, couldn't find that vertex. Can you try again?\n";
                repeat = true;
            }
        } while (repeat);

        while (runInitial());
    }

    int runInitial() {
        cout << "Ok, now an important matter for us to analyse the best path:\n"
                "Can the group being transported be separated?\n"
                "1 - Yes\n"
                "2 - No\n"
                "3 - Re-select origin and target\n"
                "0 - Exit\n";
        switch (intInput(0, 3)) {
            case 1:
                while (runSecond());
                return 0;
            case 2:
                while (runFirst());
                return 0;
            case 3:
                runSelectOrigin();
                return 0;
            case 0:
                return 0;
        }
        return 1;
    }

    int runFirst() {
        graph.paths.clear();
        cout << "Ok! Now that we know that groups can't be separated, "
                "tell me what we want to find.\n"
                "1 - The biggest possible group to go from origin to destination\n"
                "2 - The best solutions in terms of group dimension and transporting shift count\n"
                "3 - Return\n"
                "0 - Exit\n";
        pair<vector<int>, int> res;
        vector<int> empty;
        switch (intInput(0, 3)) {
            case 1:
                res.second = graph.firstAlgorithm(origin, target);
                res.first = graph.getPath(origin, target);
                graph.paths.insert(res);
                graph.printPath(graph.paths);
                cout << endl;
                break;
            case 2:
                graph.paretoOptimalGroupSizeAndTransportShift(origin, target);
                if (graph.paths.size() > 1) {
                    cout << "Seems like we've found more than one pareto optimal solution. They go as follows:\n";
                }
                graph.printPath(graph.paths);
                cout << endl;
                break;
            case 3:
                while (runInitial());
                return 0;
            case 0:
                return 0;
        }
        return 1;
    }

    int runSecond() {
        graph.paths.clear();
        int groupSize;
        cout << "Ok! Now that we know that groups can be separated, "
                "tell me what to find.\n"
                "1 - Find a path to a group, given it's dimension (2.1)\n"
                "2 - Find the biggest possible group dimension and it's path (2.3)\n"
                "3 - Return\n"
                "0 - Exit\n";
        switch (intInput(0, 3)) {
            case 1:
                cout << "For this scenario, it's important to know the group size.\n"
                        "Can you tell me what is it?\n";
                groupSize = intInput(1, INT32_MAX);
                graph.FindPathGivenGroupSize(origin, target, groupSize);
                graph.printPath(graph.paths);
                cout << endl;
                break;
            case 2:
                graph.edmondKarpFlux(origin, target);
                graph.printPath(graph.paths);
                cout << endl;
                break;
            case 3:
                while (runInitial());
                return 0;
            case 0:
                return 0;
            default:
                break;
        }
        while (runAfterSecond());
        return 0;
    }

    int runAfterSecond() {
        int increase;
        cout << "Ok, now by the end of the last algorithm, we can work some mor options, if you wish.\n";
        cout << "1 - Correct a pathing due to an increase of a given group size (2.2)\n"
                "2 - Tell the soonest possible time the group would reunite in destination (2.4)\n"
                "3 - Assuming everybody leaves from the same place and time, what would "
                "be the greatest waiting time possible (2.5)\n"
                "4 - Return\n"
                "0 - Exit\n";
        switch (intInput(0, 5)) {
            case 1:
                cout << "And how much are you planning to increase the group size by?\n";
                increase = intInput(1, INF);
                graph.increaseGroupSize(origin, target, increase);
                graph.printPath(graph.paths);
                cout << endl;
                break;
            case 2:
                cout << "The soonest reunion would be in " << graph.longestPath(origin, target) << " minutes.\n";
                cout << endl;
                break;
            case 3:
                graph.vertexTime(origin, target);
                cout << endl;
                break;
            case 4:
                while (runSecond());
                return 0;
            case 0:
                return 0;
        }
        return 1;
    }
};

#endif //PROJ2_MENU_H
