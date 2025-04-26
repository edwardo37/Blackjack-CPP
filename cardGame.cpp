//
// Created by Cameron on 2/12/2025.
//

#include <iostream>
#include <deque>

// For shuffling
#include <random>
#include <algorithm>

#include "card.h"
#include "player.h"

// Lookup tables, for convenience
namespace pack52 {
    const char* SUITS[] = {
        "heart", "diamond", "club", "spade"
    }; constexpr int NUM_SUITS = 4;

    const char* RANKS[] = {
        "ace", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "jack", "queen", "king"
    }; constexpr int NUM_RANKS = 13;
}


Card::Card(const int s, const int r, Deck * o) : suit(s), rank(r), owner_(o) {}

Deck::Deck() {
    std::cout << "Creating deck, with " << pack52::NUM_SUITS << " suits and " << pack52::NUM_RANKS << " ranks.\n" << std::endl;
    for (int s=1; s<=pack52::NUM_SUITS; ++s) {
        for (int r=1; r<=pack52::NUM_RANKS; ++r) {
            cards_.push_back(new Card(s, r, this));
        }
    }
}

Deck::~Deck() {
    std::cout << "Destroying deck.\n" << std::endl;
    for (const Card* card : cards_) {
        delete card;
    }
}


void Deck::shuffle() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::ranges::shuffle(cards_.begin(), cards_.end(), gen);
}


const Card * Deck::drawCard() {
    std::cout << "Retrieving card from deck...\n";

    if (cards_.empty()) {
        std::cerr << "ERROR: No cards in deck!\n";
        return nullptr;
    }

    const Card * newCard = cards_.front();
    cards_.pop_front();

    std::cout << std::endl;

    return newCard;
}
// At first, I thought pop deallocated an object,
// then I realized I am an idiot who does not know how values and references work
void Player::drawCard(Deck & deck) {
    std::cout << "Drawing card...\n" << std::endl;

    hand_.push_front(deck.drawCard());
}

void Deck::_print() const {
    std::cout << "Cards in deck:\n";

    if (cards_.empty()) {
        std::cout << "No cards in deck!\n" << std::endl;
        return;
    }

    for (const Card * card : cards_) {
        std::cout << pack52::RANKS[card->rank-1] << " of " << pack52::SUITS[card->suit-1] << "s\n";
    }

    std::cout << std::endl;
}


Player::Player() = default;

Player::~Player() {
    std::cout << "Destroying player. Discarding all cards.\n";

    if (hand_.empty()) {
        std::cout << "Hand already discarded... Continuing to destroy player.\n";
        return;
    }

    for (const Card * card : hand_) {
        discardCard(0);
    }

    std::cout << std::endl;
}


void Deck::discardCard(const Card * card) {
    std::cout << "Discarding card to deck...\n" << std::endl;
    cards_.push_back(card);
}
void Player::discardCard(const int index) {
    std::cout << "Discarding card...\n";

    if (index >= hand_.size()) {
        std::cerr << "ERROR: Index greater than size of hand!\n";
        return;
    }

    const Card * card = hand_[index];

    card->owner_->discardCard(card);
    // Every card is unique in the hand, so I don't feel as bad using erase
    std::erase(hand_, card);

    std::cout << std::endl;
}

const Card * Player::operator[](const int index) const {
    std::cout << "Accessing card in hand...\n";

    if (hand_.empty()) {
        std::cerr << "ERROR: No cards in hand!\n";
        return nullptr;
    }

    if (index >= hand_.size()) {
        std::cerr << "ERROR: Index out of bounds.\n";
        return nullptr;
    }

    std::cout << std::endl;

    return hand_[index];
}

void Player::printHand() const {
    std::cout << "Cards in hand:\n";

    if (hand_.empty()) {
        std::cout << "Hand is empty!\n" << std::endl;
        return;
    }

    for (int c=1; c<=hand_.size(); ++c) {
        std::cout << c << ". " << pack52::RANKS[hand_[c-1]->rank-1] << " of " << pack52::SUITS[hand_[c-1]->suit-1] << "s\n";
    }

    std::cout << std::endl;
}
