/**
 * @file actions.h
 * @brief A file depicting the possible actions each tank could perform.
 * The maximal number of columns in a comment-line in this file is 106.
 */

// Action.h
#pragma once
#include <string>
#include <stdexcept>

enum class Action {
    Forward,
    Backward,
    Rotate8Left,
    Rotate8Right,
    Rotate4Left,
    Rotate4Right,
    Shoot,
    NoAction
};

inline std::string actionToString(Action action) {
    switch (action) {
        case Action::Forward: return "Forward";
        case Action::Backward: return "Backward";
        case Action::Rotate8Left: return "Rotate8Left";
        case Action::Rotate8Right: return "Rotate8Right";
        case Action::Rotate4Left: return "Rotate4Left";
        case Action::Rotate4Right: return "Rotate4Right";
        case Action::Shoot: return "Shoot";
        case Action::NoAction: return "NoAction";
        default: throw std::invalid_argument("Unknown Action");
    }
}

inline Action stringToAction(const std::string& str) {
    if (str == "Forward") return Action::Forward;
    if (str == "Backward") return Action::Backward;
    if (str == "Rotate8Left") return Action::Rotate8Left;
    if (str == "Rotate8Right") return Action::Rotate8Right;
    if (str == "Rotate4Left") return Action::Rotate4Left;
    if (str == "Rotate4Right") return Action::Rotate4Right;
    if (str == "Shoot") return Action::Shoot;
    if (str == "NoAction") return Action::NoAction;
    throw std::invalid_argument("Invalid action string: " + str);
}