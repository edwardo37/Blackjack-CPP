//
// Created by Cameron on 2/12/2025.
//

#include <iostream>

// For shuffling
#include <random>
#include <algorithm>

#include "card.h"
#include "player.h"

Card::Card(const int s, const int r, Deck * o) : suit(s), rank(r), owner_(o) {}

Deck::Deck() {
    for (int s=1; s<=pack52::NUM_SUITS; ++s) {
        for (int r=1; r<=pack52::NUM_RANKS; ++r) {
            cards_.push_back(new Card(s, r, this));
        }
    }
}

Deck::~Deck() {
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
    if (cards_.empty()) {
        return nullptr;
    }

    const Card * newCard = cards_.front();
    cards_.pop_front();

    return newCard;
}
void Player::drawCard(Deck & deck) {
    hand_.push_back(deck.drawCard());
}

void Deck::_print() const {
    if (cards_.empty()) {
        return;
    }

    for (const Card * card : cards_) {
        std::cout << pack52::RANKS[card->rank-1] << " of " << pack52::SUITS[card->suit-1] << "s\n";
    }

    std::cout << std::endl;
}


Player::Player(const char* n="PLAYER") : name(n), score(0), isOut(false) {}

Player::~Player() {
    if (hand_.empty()) {
        return;
    }

    for (int i=0; i<=hand_.size(); ++i) {
        discardCard(0);
    }
}


void Deck::discardCard(const Card * card) {
    cards_.push_back(card);
}
void Player::discardCard(const int index) {
    if (index >= hand_.size()) {
        std::cerr << "ERROR: Index greater than size of hand!" << std::endl;
        exit(1);
    }

    const Card * card = hand_[index];

    card->owner_->discardCard(card);
    // Every card is unique in the hand, so I don't feel as bad using erase()
    std::erase(hand_, card);
}

const Card * Player::operator[](const int index) const {
    if (hand_.empty()) {
        std::cerr << "ERROR: No cards in hand!\n";
        exit(1);
    }

    if (index >= hand_.size()) {
        std::cerr << "ERROR: Index out of bounds.\n";
        exit(1);
    }

    return hand_[index];
}

void Player::printHand() const {
    if (hand_.empty()) {
        std::cout << "Hand is empty!" << std::endl;
        return;
    }

    for (int c=1; c<=hand_.size(); ++c) {
        std::cout << c << ". " << pack52::RANKS[hand_[c-1]->rank-1] << " of " << pack52::SUITS[hand_[c-1]->suit-1] << "s\n";
    }

    std::cout << std::endl;
}

unsigned long Player::cardCount() const {
    return hand_.size();
}

