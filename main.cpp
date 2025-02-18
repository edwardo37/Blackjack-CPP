#include <iostream>

#include "card.h"
#include "player.h"

using namespace std;

int main() {
    Deck deck;

    Player player1;
    player1.drawCard(deck);

    deck.print();
    player1.printHand();

    return 0;
}