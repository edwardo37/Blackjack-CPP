//
// Created by Cameron on 2/12/2025.
//

#ifndef CARD_H
#define CARD_H

// Fitting, I know
#include <deque>

class Deck;

// A common playing card class
class Card {
public:
    const int suit;
    const int rank;

    // Each card belongs to a deck, so when it is discarded, it goes back into the same deck.
    Deck * owner_;

    Card(int, int, Deck *);
};

class Deck {
    std::deque<const Card *> cards_;
public:
    Deck();
    ~Deck();

    void shuffle();

    // Get a card from this deck
    const Card * drawCard();
    // Discard a card back into the deck it is owned by
    void discardCard(const Card *);

    void _print() const;
};

#endif //CARD_H
