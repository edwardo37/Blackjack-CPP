#include <iostream>
#include <vector>

#include <algorithm>
#include <random>

#include <limits>

#include "card.h"
#include "player.h"

// For safe(er) memory management with a dynamic list of players
class PlayerList {
public:
    explicit PlayerList(const int numPlayers) {
        for (int i = 0; i < numPlayers; ++i) {
            contents.push_back(new Player);
        }
    }

    ~PlayerList() {
        for (const auto p : contents) {
            delete p;
        }
    }

    Player* operator[](const int index) const {
        return contents[index];
    }

    std::vector<Player*> contents;
};


int main() {
    Deck deck;
    deck.shuffle();

    std::vector<Player> players;

    // Pick number of players
    int numPlayers = 0;
    while (!numPlayers) {
        std::cout << "Enter number of players: ";
        std::cin >> numPlayers;

        // Check if valid
        if (!std::cin || numPlayers < 2 || numPlayers > 7) {
            std::cout << "Number of players must be between 2 and 7." << std::endl;

            // Reset and continue
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            numPlayers = 0;
        }
    }

    // Create all players
    PlayerList BJ_Players(numPlayers);
    Player Dealer;



    return 0;
}