#include <functional>
#include <iostream>
#include <vector>

#include <limits>

#include "card.h"
#include "player.h"


// Check the score. Also includes ace promotions. Returns if
void updateScore(Player& player) {
    // To check for the possibility of ace promotions later.
    int possibleUniqueScores = 1;

    // Add all values to total score.
    for (int i = 0; i < player.cardCount(); ++i) {
        // If an ace, mark it for later
        if (player[i]->rank == 1) {
            ++possibleUniqueScores;
        }

        // Add the smallest score
        player.score += player[i]->rank;
    }

    // Attempt to replace aces of value 1 with 11, if it doesn't go over 21
    // Why the player would choose to put themselves in trouble is beyond me
    for (int u = 1; u < possibleUniqueScores; ++u) {
        // Promote ace value -- score is promotable without going over
        if (player.score + 10 <= 21) {
            player.score += 10;
        }
    }
}


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
    const PlayerList BJ_Players(numPlayers);
    Player Dealer;

    // Draw the first two cards for everyone
    for (int i = 0; i < numPlayers; ++i) {
        BJ_Players[i]->drawCard(deck);
        BJ_Players[i]->drawCard(deck);
        updateScore(*BJ_Players[i]);
    }

    // Draw two for the dealer.
    // Only the first will be displayed on each player's turn,
    // so the player can decide to hit or stand
    for (int i = 0; i < 2; ++i) {
        Dealer.drawCard(deck);
    }
    updateScore(Dealer);


    // Check if dealer gets blackjack before first round starts
    if (Dealer.score == 21)
    {
        std::cout << "DEALER's hand:" << std::endl;
        Dealer.printHand();
        std::cout << "DEALER won. DEALER got a blackjack!\n";
        return 0;
    }

    return 0;
}