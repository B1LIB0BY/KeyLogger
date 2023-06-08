#ifndef KEYCODE_TO_ASCII_H
#define KEYCODE_TO_ASCII_H

#include <unordered_map>
#include <string>

std::string getKeycodeToAscii(int keycode) {
    static const std::unordered_map<int, std::string> keyMap = {
        {1, " {ESC} "},
        {2, "1"},
        {3, "2"},
        {4, "3"},
        {5, "4"},
        {6, "5"},
        {7, "6"},
        {8, "7"},
        {9, "8"},
        {10, "9"},
        {11, "0"},
        {14, " {DEL} "},
        {15, "  "},
        {16, "Q"},
        {17, "W"},
        {18, "E"},
        {19, "R"},
        {20, "T"},
        {21, "Y"},
        {22, "U"},
        {23, "I"},
        {24, "O"},
        {25, "P"},
        {26, "["},
        {27, "]"},
        {28, " {ENTER} \n"},
        {30, "A"},
        {31, "S"},
        {32, "D"},
        {33, "F"},
        {34, "G"},
        {35, "H"},
        {36, "J"},
        {37, "K"},
        {38, "L"},
        {44, "Z"},
        {45, "X"},
        {46, "C"},
        {47, "V"},
        {48, "B"},
        {49, "N"},
        {50, "M"},
        {51, ","},
        {52, "."},
        {56, " {ALT} "},
        {57, " "}
    };

    auto it = keyMap.find(keycode);
    if (it != keyMap.end()) {
        return it->second;
    }

    return ""; // Return empty string for unrecognized keycodes
}

#endif 