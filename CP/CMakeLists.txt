cmake_minimum_required(VERSION 3.10)

project(BullsAndCows LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Пути к заголовочным файлам
include_directories(include)

# Сервер
add_executable(server
    src/server.cpp
    src/game.cpp
)

# Клиент
add_executable(client
    src/client.cpp
)

# Для Linux (сокеты)
if(UNIX)
    target_link_libraries(server PRIVATE pthread)
endif()