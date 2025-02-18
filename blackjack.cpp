#include <iostream>
#include <vector>

#include <algorithm>
#include <random>

#include "card.h"
#include "player.h"

int main() {
    Deck deck;
    deck.shuffle();

    Player player1;

    player1.drawCard(deck);
    player1.drawCard(deck);

    player1.printHand();

    player1.discardCard(0);

    player1.printHand();

    return 0;
}