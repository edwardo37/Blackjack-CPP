//
// Created by Cameron on 2/12/2025.
//

#ifndef CARD_H
#define CARD_H

// Fitting, I know
#include <deque>

class Deck;

class Card {
public:
    const int suit;
    const int rank;

    Deck * owner_;

    Card(int, int, Deck *);
    explicit Card(const Card *);
};

class Deck {
    std::deque<const Card *> cards_;
public:
    Deck();
    ~Deck();

    void shuffle();

    const Card * drawCard();
    void discardCard(const Card *);

    void print() const;
};

#endif //CARD_H
