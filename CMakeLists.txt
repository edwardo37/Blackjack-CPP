cmake_minimum_required(VERSION 3.30)
project(Blackjack)

set(CMAKE_CXX_STANDARD 20)

add_executable(Blackjack blackjack.cpp
        card.h
        player.h
        cardGame.cpp)
