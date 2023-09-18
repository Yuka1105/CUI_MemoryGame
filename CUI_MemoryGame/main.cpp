#include <windows.h>
#include "Game.h"
using namespace std;

int main() {
	Game game;
	while (game.currentTransState != GameState::EXIT) {
		system("cls"); // ‰æ–Ê‚ðƒNƒŠƒA
		switch (game.currentTransState) {
		case GameState::TITLE:
			game.display_title();
		case GameState::GAME:
			game.start();
		case GameState::RESULT:
			game.display_result();
		}
	}
	return EXIT_SUCCESS;
}