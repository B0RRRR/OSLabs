#pragma once
#include <string>


class Game {
public:
Game(int players);


bool is_full() const;
bool started() const;


int add_player(int socket);
int current_player() const;


std::string process_guess(const std::string& guess, bool& win);


private:
int max_players;
int turn;
std::string secret;


std::string generate_secret();
};