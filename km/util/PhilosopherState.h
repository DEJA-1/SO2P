//
// Created by Kamil Majewski on 20/03/2025.
//

#ifndef PHILOSOPHERSTATE_H
#define PHILOSOPHERSTATE_H
#include <string>

enum class PhilosopherState {
    THINKING,
    HUNGRY,
    EATING
};

inline std::string stateToString(PhilosopherState state) {
    switch (state) {
        case PhilosopherState::THINKING: return "Thinking";
        case PhilosopherState::HUNGRY: return "Hungry";
        case PhilosopherState::EATING: return "Eating";
        default: return "UNKNOWN";
    }
}

#endif //PHILOSOPHERSTATE_H
