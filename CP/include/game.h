#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>

class Game {
public:
    // *** ИЗМЕНЕНИЕ: Уменьшаем длину кода до 3 ***
    static const int CODE_LENGTH = 3;

    Game(int players);
    
    // Обрабатывает ход игрока и возвращает строку результата (RESULT B C)
    std::string process_guess(const std::string& guess, bool& win);

private:
    std::string secret_code;
    int num_players;

    // Генерирует секретный код
    void generate_code();

    // Проверяет ход и возвращает пару {bulls, cows}
    std::pair<int, int> check_guess(const std::string& guess);

    // Проверяет, состоит ли строка из CODE_LENGTH уникальных цифр
    bool is_valid(const std::string& guess);
};

#endif // GAME_H