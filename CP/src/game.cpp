#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include <sstream>
#include <set>

#include "game.h"

Game::Game(int players) : num_players(players) {
    generate_code();
    std::cout << "DEBUG: Secret code set to: " << secret_code << std::endl;
}

bool Game::is_valid(const std::string& guess) {
    if (guess.length() != CODE_LENGTH) {
        return false;
    }
    
    for (char c : guess) {
        if (!isdigit(c)) {
            return false;
        }
    }

    // Characters must be unique
    std::set<char> unique_digits(guess.begin(), guess.end());
    if (unique_digits.size() != CODE_LENGTH) {
        return false;
    }

    return true;
}

void Game::generate_code() {
    std::vector<int> digits = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(digits.begin(), digits.end(), std::default_random_engine(seed));
    
    secret_code = "";
    
    
    int start_index = 0;
    if (CODE_LENGTH > 1 && digits[0] == 0) {
        auto it = std::find_if(digits.begin() + 1, digits.end(), [](int d){ return d != 0; });
        if (it != digits.end()) {
            std::swap(digits[0], *it);
        }
    }

    for (int i = 0; i < CODE_LENGTH; ++i) {
        secret_code += std::to_string(digits[i]);
    }
}

std::pair<int, int> Game::check_guess(const std::string& guess) {
    int bulls = 0;
    int cows = 0;

    for (int i = 0; i < CODE_LENGTH; ++i) {
        for (int j = 0; j < CODE_LENGTH; ++j) {
            if (guess[i] == secret_code[j]) {
                if (i == j) {
                    bulls++;
                } else {
                    cows++;
                }
            }
        }
    }
    return {bulls, cows};
}

std::string Game::process_guess(const std::string& guess, bool& win) {
    win = false;

    if (!is_valid(guess)) {
        return "RESULT 0 0 (Invalid)";
    }

    std::pair<int, int> result = check_guess(guess);
    int bulls = result.first;
    int cows = result.second;

    if (bulls == CODE_LENGTH) {
        win = true;
    }

    std::stringstream ss;
    ss << "RESULT " << bulls << " " << cows;
    return ss.str();
}