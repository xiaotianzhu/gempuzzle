//
// Menu.cpp -- function definition for menus
//
// Written by Xiaotian Zhu
//

#include <iostream>
#include "Game.h"
#include <limits>

#define FOUND -100
#define NOTFOUND -200

void welcomeMenu(){
	std::cout << std::endl << "####GEM####";
    std::cout << std::endl << "## 1 2 3 ##";
    std::cout << std::endl << "## 4 5 6 ##";
	std::cout << std::endl << "## 7 8   ##";
	std::cout << std::endl << "###PUZZLE##" << std::endl;
	std::cout << std::endl << "Have Fun :)" << std::endl;
    /*std::cout << std::endl << "Welcome, Welcome, Welcome,";
    std::cout << std::endl << "To This Acient Gem Puzzle.";
    std::cout << std::endl << "Archimedes Is To Be Amazed,";
	std::cout << std::endl << "As Well As Euler And Pascal,";
	std::cout << std::endl << "To Watch Thou Always Discover,";
	std::cout << std::endl << "An Optimal Path So Magical :)" << std::endl;*/
}

void gameMenu(){
    Game aNewGame(3, 3);
    std::cout << std::endl << "Current Puzzle:" << std::endl;
    aNewGame.GetCurrentPuzzle() -> Display();
    bool exit = false;
    int choice, previousLength, currentLength;
    while(!exit){
        std::cout << std::endl << "Game Menu:";
        std::cout << std::endl << "1. Play; 2. Solve for Me; 3. Game Info; 4. Replay; 5. Main Menu." << std::endl;

        for(;;){
            if (std::cin >> choice && choice > 0 && choice < 6) {
                break;
            } else {
                std::cout << std::endl << "Please enter 1, 2, 3, 4 or 5:";
                std::cout << std::endl << "1. Play; 2. Solve for Me; 3. Game Info; 4. Replay; 5. Main Menu." << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }

        switch(choice){
            case 1:
                std::cout << std::endl << "Current Puzzle:" << std::endl;
                aNewGame.GetCurrentPuzzle() -> Display();
                aNewGame.MoveInterface();
                break;

            case 2:
				previousLength = aNewGame.GetMoveHistory().size();
                if(aNewGame.SolveIt() == NOTFOUND){
                    std::cout << std::endl << "I bet 1000$ this is NOT solvable ;)" << std::endl;
                }else{
					currentLength = aNewGame.GetMoveHistory().size();
                    std::cout << std::endl << "Got it :) Shortest solution (" << currentLength - previousLength << " steps) appended to Move History:" << std::endl;
                    aNewGame.Display();
                }
                break;

            case 3:
                std::cout << std::endl << "Game Info:" << std::endl;
                aNewGame.Display();
                break;

			case 4:
				aNewGame.Replay();
				break;

            case 5:
                exit = true;
                break;
            default:;
        }

    }

}

void mainMenu(){
    bool exit = false;
    int choice;
    while (!exit){
        std::cout << std::endl << "Main Menu:";
        std::cout << std::endl << "1. New Game; 2. Exit." << std::endl;

        for(;;){
            if (std::cin >> choice && choice > 0 && choice < 3) {
                break;
            } else {
                std::cout << std::endl << "Please enter 1 or 2:";
                std::cout << std::endl << "1. New Game; 2. Exit." << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }

        switch(choice){
            case 1:
                gameMenu();
                break;
            case 2:
                exit = true;
                std::cout << std::endl << "See you :)" << std::endl << std::endl;
                break;
            default:;
        }

    }
}
