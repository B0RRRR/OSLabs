#include "game.h"
#include <algorithm>
#include <random>


Game::Game(int players)
: max_players(players), turn(0), secret(generate_secret()) {}


std::string Game::generate_secret() {
std::string d = "0123456789";
std::shuffle(d.begin(), d.end(), std::mt19937{std::random_device{}()});
return d.substr(0, 4);
}


bool Game::is_full() const {
return true; // сервер контролирует количество подключений
}


bool Game::started() const {
return true;
}


int Game::add_player(int) {
return 0;
}


int Game::current_player() const {
return turn;
}


std::string Game::process_guess(const std::string& guess, bool& win) {
int bulls = 0, cows = 0;


for (int i = 0; i < 4; ++i) {
if (guess[i] == secret[i]) bulls++;
else if (secret.find(guess[i]) != std::string::npos) cows++;
}


if (bulls == 4) {
win = true;
return "WIN";
}


win = false;
turn = (turn + 1) % max_players;
return "RESULT " + std::to_string(bulls) + " " + std::to_string(cows);
}