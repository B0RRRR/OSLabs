#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <signal.h>
#include <sys/select.h>
#include <sys/time.h>
#include <algorithm>
#include <stdexcept>

#include "game.h"


volatile sig_atomic_t keep_running = 1;

void signal_handler(int sig) {
    if (sig == SIGINT) {
        keep_running = 0;
        std::cout << "\nSIGINT received. Shutting down gracefully..." << std::endl;
    }
}

void broadcast(const std::vector<int>& clients, const std::string& message) {
    std::string msg_with_newline = message + "\n";
    for (int c : clients) {
        send(c, msg_with_newline.c_str(), msg_with_newline.size(), 0);
    }
}

int main(int argc, char* argv[]) {
    
    signal(SIGINT, signal_handler);

    const int DEFAULT_PLAYERS = 2;
    int players_required = DEFAULT_PLAYERS;

    // Read cmd arguments
    if (argc == 2) {
        try {
            int requested_players = std::stoi(argv[1]);
            
            if (requested_players > 1) {
                players_required = requested_players;
            } else {
                std::cerr << "Error: Number of players must be greater than 1. Using default (" << DEFAULT_PLAYERS << ")." << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error parsing player count argument. Using default (" << DEFAULT_PLAYERS << ")." << std::endl;
        }
    }

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    // ports/adresses can be used again
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        close(server_fd);
        return 1;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5555);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Bind failed (port 5555 likely in use)");
        close(server_fd);
        return 1;
    }

    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        close(server_fd);
        return 1;
    }

    std::cout << "Server started on port 5555. Required players: " << players_required << ". Waiting for clients..." << std::endl;

    std::vector<int> clients;

    // Find players
    while ((int)clients.size() < players_required && keep_running) {
        fd_set master_set;
        FD_ZERO(&master_set);
        FD_SET(server_fd, &master_set);
        
        struct timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        if (select(server_fd + 1, &master_set, nullptr, nullptr, &timeout) < 0) {
            if (keep_running) perror("Select error during connect");
            break;
        }
        
        if (FD_ISSET(server_fd, &master_set)) {
            int c = accept(server_fd, nullptr, nullptr);
            if (c < 0) continue;
            
            clients.push_back(c);
            std::cout << "Client connected: socket = " << c << std::endl;

            std::string id_msg = "YOUR_ID " + std::to_string(c);
            send(c, id_msg.c_str(), id_msg.size(), 0); 
            
            std::string wait_msg = "WAIT " + std::to_string(players_required - clients.size());
            send(c, wait_msg.c_str(), wait_msg.size(), 0);
        }
    }
    
    // Game process
    if ((int)clients.size() == players_required && keep_running) {
        
        std::cout << "All players connected. Starting game" << std::endl;
        Game game(players_required);
        bool win = false; 
        char buf[128];
        
        broadcast(clients, "START");

        while (!win && keep_running) {
            for (int i = 0; i < players_required; ++i) {
                if (!keep_running) break;

                int sock = clients[i];
                
                std::string turn_msg = "TURN " + std::to_string(sock);
                broadcast(clients, turn_msg);
                
                fd_set read_set;
                FD_ZERO(&read_set);
                FD_SET(sock, &read_set);
                
                struct timeval timeout_turn;
                timeout_turn.tv_sec = 60;
                timeout_turn.tv_usec = 0;
                
                int ready_fds = select(sock + 1, &read_set, nullptr, nullptr, &timeout_turn);
                
                if (!keep_running) break;
                
                if (ready_fds <= 0) {
                    std::cout << "Client " << sock << " timed out or select error. Aborting." << std::endl;
                    broadcast(clients, "SERVER_SHUTDOWN");
                    keep_running = 0; 
                    break; 
                }

                memset(buf, 0, sizeof(buf));
                int r = recv(sock, buf, sizeof(buf)-1, 0);
                
                if (r <= 0) {
                    std::cout << "Client " << sock << " disconnected unexpectedly (r=" << r << ")." << std::endl;
                    broadcast(clients, "SERVER_SHUTDOWN"); 
                    keep_running = 0; 
                    break;
                }
                
                std::string guess(buf);
                guess.erase(std::remove_if(guess.begin(), guess.end(), [](char c){ return c == '\n' || c == '\r'; }), guess.end());
                
                if (guess == "QUIT") {
                    std::cout << "Client " << sock << " requested QUIT. Aborting game." << std::endl;
                    broadcast(clients, "SERVER_SHUTDOWN");
                    keep_running = 0;
                    break;
                }

                std::cout << "Received guess from client " << sock << ": " << guess << std::endl;

                std::string resp = game.process_guess(guess, win);
                
                broadcast(clients, resp);

                if (win) {
                    std::string win_msg = "WIN " + std::to_string(sock);
                    broadcast(clients, win_msg);
                    break;
                }
            }
        }
    } else {
        broadcast(clients, "SERVER_SHUTDOWN");
    }

    std::cout << "Game over." << std::endl;
    
    for(int c : clients) {
        close(c);
    }
    
    close(server_fd);
    std::cout << "Server successfully shut down." << std::endl;
    
    return 0;
}