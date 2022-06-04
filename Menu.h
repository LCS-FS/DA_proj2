//
// Created by jpdat on 04-06-2022.
//

#ifndef PROJ2_MENU_H
#define PROJ2_MENU_H

#include <map>
#include <vector>
#include "Graph.h"

class IState {
private:
    string content;
public:
    int minimalOption;
    int maximumOption;
    void View() {std::cout << content;}
    virtual int ProcessInput(int input);
    static int intInput(int min, int max);
    void setContent(string toSet) {content = toSet;}
};

class InitialState : public IState {
public:
    int minimalOption = 1;
    int maximumOption = 2;
    InitialState() { setContent("Welcome to the Path Finder! Your algorithmic interface to find best paths through"
                                " Graph data structures :)\n"
                                "First of all, an important matter for us to analyse the best path:\n"
                                "Can the group being transported be separated?\n"
                                "1 - Yes\n"
                                "2 - No\n");
                    minimalOption = 1;
                    maximumOption = 2;
    }
    int ProcessInput(int input) override {
        if (input == 1) {
            return 2;
        } else {
            return 1;
        }
    }
};

class ScenarioOne : public IState {
public:

    ScenarioOne() { setContent("Ok! Now that we know that groups can't be separated, "
                               "tell me what we want to find.\n"
                               "1 - The biggest possible group to go from origin to destination\n"
                               "2 - The best solutions in terms of group dimension and transporting shift count\n");
                    maximumOption = 2;
                    minimalOption = 1;
    }
    int ProcessInput(int input) override {
        if (input == 1) {
            //run algorithm for 1.1
        } else {
            //run algorithm for 1.2
        }
        return -1;
    }
};

class ScenarioTwo : public IState {
public:
    int minimalOption = 1;
    int maximumOption = 5;
    ScenarioTwo() { setContent("Ok! Now that we know that groups can be separated, "
                               "tell me what to find.\n"
                               "1 - Find a path to a group, given it's dimension\n"
                               "2 - Correct a pathing due to an increase of a given group size\n"
                               "3 - Find the biggest possible group dimension and it's path\n"
                               "4 - Tell the soonest possible time the group would reunite in destination\n"
                               "5 - Assuming everybody leaves from the same place and time, what would "
                               "be the greatest waiting time possible\n");
                    minimalOption = 1;
                    maximumOption = 5;
    }
    int ProcessInput(int input) override {
        switch (input) {
            case 1:
                //run algorithm 2.1
                break;
            case 2:
                //run algorithm 2.2
                break;
            case 3:
                //run algoritm 2.3
                break;
            case 4:
                //run algorithm 2.4
                break;
            case 5:
                //run algorithm 2.5
                break;
            default:
                break;
        }
        return -1;
    }
};

class Menu {
public:
    vector<IState> states;
    IState* currentState;
    Graph<int> graph;
    Menu() {
        InitialState initialState;
        ScenarioOne scenarioOne;
        ScenarioTwo scenarioTwo;
        states.push_back(initialState); states.push_back(scenarioOne); states.push_back(scenarioTwo);
    }
    void run() {
        int code = 0;
        while (code != -1) {
            currentState = &states[code];
            currentState->View();
            code = currentState->ProcessInput(IState::intInput(currentState->minimalOption, currentState->maximumOption));
        }
    }
};


#endif //PROJ2_MENU_H
