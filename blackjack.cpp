#include <iostream>
#include <vector>

#include <algorithm>
#include <random>

#include "card.h"
#include "player.h"

int main() {
    Deck deck;
    Player player1;

    player1.drawCard(deck);

    player1.printHand();

    return 0;
}