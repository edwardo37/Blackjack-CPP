#include <iostream>
#include <vector>

#include <limits>

#include "card.h"
#include "player.h"
#include "logger.h"

#ifdef _WIN32
    auto NULL_DEVICE = "NUL";
#else
    auto NULL_DEVICE = "/dev/null";
#endif


// Check the score. Also includes ace promotions.
void updateScore(Player& player) {
    // To check for the possibility of ace promotions later.
    int possibleUniqueScores = 1;

    // Add all values to the total score.
    for (int i = 0; i < player.cardCount(); ++i) {
        // If an ace, mark it for later
        if (player[i]->rank == 1) {
            ++possibleUniqueScores;
        }

        // J, Q, K
        if (player[i]->rank > 10) {
            player.score += 10;
        }
        // Face value (aces included for now)
        else {
            player.score += player[i]->rank;
        }
    }

    // Attempt to replace aces of value 1 with 11 if it doesn't go over 21
    // Why the player would choose to put themselves in trouble is beyond me
    for (int u = 1; u < possibleUniqueScores; ++u) {
        // Promote ace value -- the score is promotable without going over
        if (player.score + 10 <= 21) {
            player.score += 10;
        }
    }
}


// For safe(er) memory management with a dynamic list of players
class PlayerList {
public:
    explicit PlayerList(const int numPlayers, Logger& logger) {
        for (int i=0; i < numPlayers; ++i) {
            char name[16];
            while (true) {
                std::cout << "Enter player " << i+1 << "'s name: ";
                std::cin.getline(name, 16);

                if (std::cin) {break;}

                // Invalid input
                logger.log(Logger::WARNING, "User entered invalid name.");
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input." << std::endl;
            }

            contents.push_back(new Player(name));
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


int main(int argc, char** argv) {
    Logger logger;

    // Check arguments
    if (argc > 1) {
        for (int i = 1; i < argc; ++i) {
            if (argv[i][0] == '-' && argv[i][1] == 'l') {
                logger.openFile("blackjack.log");
            }
            else {
                logger.openFile(NULL_DEVICE);
            }
        }
    }

    logger.log("Starting Blackjack.");
    std::cout << "Welcome to Blackjack!" << std::endl;

    logger.log("Initializing and shuffling 52-card deck.");
    Deck deck;
    deck.shuffle();

    std::vector<Player> players;

    // Pick a number of players
    logger.log("User picking number of players.");
    int numPlayers = 0;
    while (!numPlayers) {
        std::cout << "Enter number of players: ";
        std::cin >> numPlayers;

        // Check if valid
        if (!std::cin || numPlayers < 2 || numPlayers > 7) {
            logger.log(Logger::WARNING, "User entered invalid number of players.");
            std::cout << "Number of players must be between 2 and 7." << std::endl;

            // Reset and continue
            numPlayers = 0;
        }

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // Create all players
    logger.log("Creating all players and dealer.");
    const PlayerList BJ_Players(numPlayers, logger);
    Player Dealer("DEALER");

    // Draw the first two cards for everyone
    logger.log("Drawing first two cards for everyone.");
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

    // Check if the dealer gets blackjack before the first round starts
    logger.log("Checking in anyone got a Blackjack...");
    if (Dealer.score == 21)
    {
        logger.log("DEALER won, with a ", pack52::RANKS[Dealer[0]->rank - 1], " and ",
                   pack52::RANKS[Dealer[1]->rank - 1], ", changing to 10 and an ace worth 11.");

        std::cout << "DEALER's hand:" << std::endl;
        Dealer.printHand();
        std::cout << "DEALER won. DEALER got a blackjack!\n";
    }
    for (auto player : BJ_Players.contents) {
        if (player->score == 21) {
            logger.log(player->name,
                " won, with a ", pack52::RANKS[(*player)[0]->rank - 1],
                " and ", pack52::RANKS[(*player)[1]->rank - 1],
                ", changing to 10 and an ace worth 11.");
            std::cout << player->name << "'s hand:" << std::endl;
            player->printHand();
            std::cout << player->name << " won. " << player->name << " got a blackjack!\n";
        }
    }

    return 0;
}