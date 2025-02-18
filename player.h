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
    Player();
    ~Player();

    void drawCard(Deck &);
    void discardCard(int);

    const Card * operator[](int) const;

    void printHand() const;
};

#endif //PLAYER_H
