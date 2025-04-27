//
// Created by Cameron on 2/12/2025.
//

#ifndef PLAYER_H
#define PLAYER_H

#include <deque>

#include "card.h"

class Player {
    std::deque<const Card *> hand_;
public:
    int score;

    Player();
    ~Player();

    // Draw a card from a deck
    void drawCard(Deck &);
    // Discard a card in the hand by its index
    void discardCard(int);

    // Access a card by its index
    const Card * operator[](int) const;

    // Print all cards in the hand
    void printHand() const;

    // Access the number of cards
    [[nodiscard]] unsigned long cardCount() const;
};

#endif //PLAYER_H
