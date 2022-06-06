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

    }

    void runSelectTarget() {
        cout << "Great, now let's select our target destination. (Input the integer representing the node)\n";
        bool repeat;
        do {
            repeat = false;
            target = intInput(1, INT32_MAX);
            if (graph.findVertex(origin) == nullptr) {
                cout << "Sorry, couldn't find that vertex. Can you try again?\n";
                repeat = true;
            }
        } while (repeat);

        runInitial();
    }

    int runInitial() {
        cout << "Ok, now an important matter for us to analyse the best path:\n"
                "Can the group being transported be separated?\n"
                "1 - Yes\n"
                "2 - No\n"
                "3 - Return\n"
                "0 - Exit\n";
        switch (intInput(0, 3)) {
            case 1:
                runFirst();
                break;
            case 2:
                runSecond();
                break;
            case 3:
                runSelectOrigin();
                break;
            case 0:
                return 0;
        }
    }

    int runFirst() {
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
                break;
            case 2:
                graph.paretoOptimalGroupSizeAndTransportShift(origin, target);
                break;
            case 3:
                runInitial();
                break;
            case 0:
                return 0;
        }
    }

    int runSecond() {
        int groupSize;
        cout << "For these scenarios, it's important to know the group size.\n"
                "Can you tell me what is it?\n";
        groupSize = intInput(1, INT32_MAX);

        cout << "Ok! Now that we know that groups can be separated, "
                "tell me what to find.\n"
                "1 - Find a path to a group, given it's dimension\n"
                "2 - Correct a pathing due to an increase of a given group size\n"
                "3 - Find the biggest possible group dimension and it's path\n"
                "4 - Tell the soonest possible time the group would reunite in destination\n"
                "5 - Assuming everybody leaves from the same place and time, what would "
                "be the greatest waiting time possible\n";
        switch (intInput(1, 5)) {
            case 1:
                graph.printPath(graph.FindPathGivenGroupSize(origin, target, groupSize));
                break;
            case 2:

                break;
            case 3:

                break;
            case 4:

                break;
            case 5:

                break;
            default:
                break;
        }
        return 0;
    }
};

#endif //PROJ2_MENU_H
