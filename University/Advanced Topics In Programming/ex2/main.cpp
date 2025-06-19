#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "gamemanager.h"
#include "MyTankAlgorithmFactory.h"
#include "MyPlayerFactory.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "no input file given!\n";
        return 1;
    }
    try {
        auto playerFactory = std::make_unique<MyPlayerFactory>();
        auto tankFactory = std::make_unique<MyTankAlgorithmFactory>();
        GameManager game(std::move(playerFactory), std::move(tankFactory));

        game.readBoard(argv[1]);
        game.run();
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;

}