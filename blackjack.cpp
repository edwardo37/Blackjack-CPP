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

const char* SUITS[] = {
    "heart", "diamond", "club", "spade"
};
constexpr int NUM_SUITS = 4;

const char* RANKS[] = {
    "ace", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "jack", "queen", "king"
};
constexpr int NUM_RANKS = 13;


Card::Card(const int s, const int r, Deck * o) : suit(s), rank(r), owner_(o) {}

Card::Card(const Card * oldCard) : suit(oldCard->suit), rank(oldCard->rank), owner_(oldCard->owner_) {}

Deck::Deck() {
    for (int s=0; s<NUM_SUITS; ++s) {
        for (int r=0; r<NUM_RANKS; ++r) {
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
    const Card * newCard = cards_.front();
    cards_.pop_front();
    return newCard;
}
// At first, I thought pop deallocated an object,
// then I realized I am an idiot who does not know how values and references work
void Player::drawCard(Deck & deck) {
    hand_.push_front(deck.drawCard());
}

const Card * Deck::operator[](const int pos) const {
    return cards_.at(pos);
}

void Deck::print() const {
    std::cout << "Cards in deck:" << std::endl;
    for (const Card * card : cards_) {
        std::cout << RANKS[card->rank] << " of " << SUITS[card->suit] << "s\n";
    }
    std::cout << std::endl;
}


Player::Player() = default;

Player::~Player() {
    for (const Card * card : hand_) {
        discardCard(card);
    }
}


void Deck::discardCard(const Card * card) {
    cards_.push_back(card);
}
void Player::discardCard(const Card * card) {
    card->owner_->discardCard(card);
    // Every card is unique in the hand, so I don't feel as bad using erase
    std::erase(hand_, card);
}

void Player::printHand() const {
    std::cout << "Cards in hand:" << std::endl;
    for (const Card * card : hand_) {
        std::cout << RANKS[card->rank] << " of " << SUITS[card->suit] << "s\n";
    }
    std::cout << std::endl;
}
