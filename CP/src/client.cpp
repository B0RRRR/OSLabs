#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <cstring>
#include <algorithm>
#include <vector>
#include <sstream>

// str -> characters
std::vector<std::string> split(const std::string &s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

void trim_message(std::string& str) {
    str.erase(std::remove_if(str.begin(), str.end(), [](char c){ return c == '\n' || c == '\r'; }), str.end());
}

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return 1;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5555);
    
    if (inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr) <= 0) {
        perror("Invalid address / Address not supported");
        close(sock);
        return 1;
    }

    std::cout << "Connecting to server..." << std::endl;

    if (connect(sock, (sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Connection Failed.");
        close(sock);
        return 1;
    }
    
    int server_client_id = -1; 

    std::cout << "Connected! Waiting for server ID..." << std::endl;

    char buf[128];
    while (true) {
        memset(buf, 0, sizeof(buf));
        
        int r = recv(sock, buf, sizeof(buf)-1, 0);
        if (r <= 0) {
            std::cout << "\nServer disconnected or error occurred." << std::endl;
            break;
        }
        
        std::string msg(buf);
        trim_message(msg);
        
        std::vector<std::string> parts = split(msg, ' ');
        std::string command = parts[0];

        
        if (command == "YOUR_ID") {
            server_client_id = std::stoi(parts[1]);
            std::cout << "Server assigned you ID: " << server_client_id << std::endl;
        }
        else if (command == "WAIT") {
            std::cout << "Server: Waiting for " << parts[1] << " more player(s)..." << std::endl;
        } 
        else if (command == "START") {
            std::cout << "\nGAME STARTED \n";
        }
        else if (command == "TURN") {
            int turn_id = std::stoi(parts[1]);
            
            if (turn_id == server_client_id) {
                std::cout << "\n>>> YOUR TURN (" << server_client_id << ") <<<" << std::endl;
                std::string guess;
                
                while (true) {
                    std::cout << "Enter your guess (3 unique digits) or QUIT: " << std::flush;
                    std::cin >> guess;
                    
                    if (guess == "QUIT") {
                        send(sock, "QUIT\n", 5, 0);
                        goto exit_cleanup; 
                    }
                    
                    std::string guess_with_newline = guess + "\n";
                    send(sock, guess_with_newline.c_str(), guess_with_newline.size(), 0);
                    break;
                }
            } else {
                std::cout << "Waiting for Player " << turn_id << "'s guess...\n";
            }
        }
        else if (command == "RESULT") {
            std::cout << "Result: Bulls: " << parts[1] << ", Cows: " << parts[2] << "\n";
        }
        else if (command == "WIN") {
            std::cout << "\nGAME OVER Player " << parts[1] << " won the game!\n";
            break;
        }
        else if (command == "SERVER_SHUTDOWN") {
             std::cout << "\nServer has been shut down. Exiting.\n";
             break;
        }
        else {
            std::cout << "Server: " << msg << std::endl;
        }
    }

exit_cleanup:
    std::cout << "Closing socket..." << std::endl;
    close(sock);
    return 0;
}