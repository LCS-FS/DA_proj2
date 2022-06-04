//
// Created by jpdat on 04-06-2022.
//

#include "Menu.h"

int IState::intInput(int min, int max) {
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