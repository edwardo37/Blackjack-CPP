#include <iostream>
#include <vector>

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
    std::cout << "---- STARTING GAME ----" << std::endl;
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
    const PlayerList BJ_Players(numPlayers);
    Player Dealer;

    // Draw the first two cards for everyone
    for (int i = 0; i < numPlayers; ++i) {
        BJ_Players[i]->drawCard(deck);
        BJ_Players[i]->drawCard(deck);
    }

    // Draw two for the dealer.
    // Only the first will be displayed on each player's turn,
    // so the player can decide to hit or stand
    for (int i = 0; i < 2; ++i) {
        Dealer.drawCard(deck);
    }

    std::cout << "---- INITIALIZED ----" << std::endl;

    // Check if dealer gets blackjack
    if  (
        // Any combo
        Dealer[0]->rank + Dealer[1]->rank == 21 ||

        // Ace promotion
        (Dealer[0]->rank == 1 && Dealer[1]->rank == 10) ||
        (Dealer[0]->rank == 10 && Dealer[1]->rank == 1)
        )
    {
        std::cout << "DEALER's hand:" << std::endl;
        Dealer.printHand();
        std::cout << "DEALER won. DEALER got a blackjack!\n";
        std::cout << "---- ENDING GAME ----" << std::endl;
        return 0;
    }

    std::cout << "----- ENDING GAME -----" << std::endl;
    return 0;
}