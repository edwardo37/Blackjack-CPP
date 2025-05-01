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
    // Reset the score
    player.score = 0;

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
    std::cout
        << "<------------------>\n"
        << "WELCOME TO BLACKJACK\n"
        << "<------------------>\n"
        << std::endl;

    logger.log("Initializing and shuffling 52-card deck.");
    Deck BJ_deck;
    BJ_deck.shuffle();

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
    const PlayerList BJ_players(numPlayers, logger);
    auto BJ_dealer = Player("DEALER");
    std::cout << "<------------------>\n" << std::endl;

    // Draw the first two cards for everyone
    logger.log("Drawing first two cards for everyone.");
    for (auto player : BJ_players.contents) {
        player->drawCard(BJ_deck);
        player->drawCard(BJ_deck);
        updateScore(*player);
    }

    // Draw two for the dealer.
    // Only the first will be displayed on each player's turn,
    // so the player can decide to hit or stand
    BJ_dealer.drawCard(BJ_deck);
    BJ_dealer.drawCard(BJ_deck);
    updateScore(BJ_dealer);

    bool gameOver = false;

    // Check if anyone gets a blackjack before the first round
    logger.log("Checking in anyone got a Blackjack...");
    if (BJ_dealer.score == 21)
    {
        gameOver = true;
    }
    for (auto player : BJ_players.contents) {
        if (player->score == 21) {
            player->isOut = true;
        }
    }

    // Game is over if the dealer gets blackjack before starting
    if (!gameOver) {
        // Loop over every player
        for (auto player : BJ_players.contents) {
            while (!player->isOut) {
                // Check if already stood
                if (player->isOut) {
                    logger.log(player->name, " is out. Continuing...");
                    break;
                }

                logger.log(player->name, " is still in.");

                // Display the dealer's hand
                std::cout << "Dealer's hand: " << std::endl
                    << "1. " << pack52::RANKS[BJ_dealer[0]->rank - 1] << " of " << pack52::SUITS[BJ_dealer[0]->suit - 1] << "s" << std::endl
                    << "2. " << "******** of ********" << std::endl;

                std::cout << player->name << "'s hand:" << std::endl;
                player->printHand();
                std::cout << "Current score: " << player->score << std::endl;

                char choice[16];
                while (true) {
                    std::cout << player->name << ": HIT or STAND? ";
                    std::cin.getline(choice, 16);

                    if (!std::cin) {
                        logger.log(Logger::WARNING, "User entered invalid input.");
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "Invalid input." << std::endl;
                        continue;
                    }

                    // Might change this later, but we'll see
                    if (choice[0] == 'h' || choice[0] == 'H') {
                        logger.log(player->name, " has chosen to hit.");

                        player->drawCard(BJ_deck);

                        std::cout << player->name << "'s new hand:" << std::endl;
                        player->printHand();

                        updateScore(*player);

                        break;
                    }
                    if (choice[0] == 's' || choice[0] == 'S') {
                        logger.log(player->name, " has chosen to stand.");
                        player->isOut = true;
                        break;
                    }
                    // Not hit or stand
                    std::cout << "What?" << std::endl;
                }

                logger.log(player->name, "'s score: ", player->score);
                std::cout << player->name << "'s score: " << player->score << std::endl;

                if (player->score > 21) {
                    logger.log(player->name, " busted.");
                    std::cout << player->name << " busted." << std::endl;
                    player->isOut = true;
                }
                if (player->score == 21) {
                    logger.log(player->name, " got a blackjack!");
                    std::cout << player->name << " got a blackjack!" << std::endl;
                    player->isOut = true;
                }
            }

            logger.log(player->name, " is ending their turn.");

            std::cout << "Press Enter to continue: ";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.clear();
            std::cout << "<------------------>\n" << std::endl;
        }
    }


    std::cout << ":[SCORING]:" << std::endl
        << "Everyone out, tallying winning scores." << std::endl;
    logger.log("Everyone out, tallying winning scores.");


    // Dealer must draw until they hit 17
    if (BJ_dealer.score < 17) {
        std::cout << "Dealer must draw until they hit 17." << std::endl;
        logger.log("Dealer must draw until they hit 17.");

        while (BJ_dealer.score < 17) {
            BJ_dealer.drawCard(BJ_deck);
            updateScore(BJ_dealer);
        }
    }

    std::cout << "DEALER's hand:" << std::endl;
    BJ_dealer.printHand();
    std::cout << "DEALER's final score: " << BJ_dealer.score << std::endl
        << "<------------------>\n" << std::endl;

    logger.log("DEALER's hand: ",
        pack52::SUITS[BJ_dealer[0]->suit - 1], " of ", pack52::RANKS[BJ_dealer[0]->rank - 1],
        " and ", pack52::SUITS[BJ_dealer[1]->suit - 1], " of ", pack52::RANKS[BJ_dealer[1]->rank - 1]);

    // Check if the dealer busted
    if (BJ_dealer.score > 21) {
        logger.log("DEALER busted.");
        std::cout << "DEALER busted." << std::endl;
    }
    // Or got a blackjack
    if (BJ_dealer.score == 21) {
        logger.log("DEALER got a blackjack!");
        std::cout << "DEALER got a blackjack!" << std::endl;
    }


    // Containers in case multiple people beat the dealer
    // Start by comparing to dealer
    std::vector<Player*> aboveDealer;
    // Break even with the dealer
    std::vector<Player*> pushed;
    // If the dealer busts
    std::vector<Player*> under21;

    // Find the highest and lowest scores.
    // Determine which to look at if everyone busted.
    for (auto player : BJ_players.contents) {
        std::cout << player->name << "'s hand:" << std::endl;
        player->printHand();

        if (player->score <= 21) {
            // Compare to dealer
            if (BJ_dealer.score <= 21) {
                // Ties the dealer and breaks even
                if (player->score == BJ_dealer.score) {
                    pushed.push_back(player);
                }

                // Players that beat the dealer
                if (player->score > BJ_dealer.score) {
                    aboveDealer.push_back(player);
                }
            }

            // In case the dealer busted
            under21.push_back(player);
        }

        logger.log(player->name, "'s final score: ", player->score);
        std::cout << player->name << "'s final score: " << player->score << std::endl
            << "<------------------>\n" << std::endl;
    }


    if (BJ_dealer.score > 21) {
        std::cout << "DEALER BUSTED! All remaining players win!" << std::endl;
        for (auto player : under21) {
            std::cout << player->name << " wins, with a score of " << player->score << std::endl;
            logger.log(player->name, " wins, with a score of ", player->score);
        }
    }
    else if (aboveDealer.empty() && pushed.empty()) {
        std::cout << "DEALER has SWEPT the table! DEALER has a score of " << BJ_dealer.score << std::endl;
        logger.log("DEALER has SWEPT the table! DEALER has a score of ", BJ_dealer.score);
    }
    else {
        for (auto player : aboveDealer) {
            logger.log(player->name, " WINS, with a score of ", player->score);
            std::cout << player->name << " WINS, with a score of " << player->score << std::endl;
        }
        for (auto player : pushed) {
            logger.log(player->name, " PUSHED, with a score of ", player->score);
            std::cout << player->name << " PUSHED, with a score of " << player->score << std::endl;
        }
    }
}