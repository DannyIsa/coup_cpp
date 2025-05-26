#include "player.hpp"

Player::Player(Game &game, string name)
    : game(game), name(name), coins(0), is_alive(true) {
  game.addPlayer(*this);
}

int Player::getCoins() { return coins; }

bool Player::isAlive() { return is_alive; }

string Player::getName() { return name; }

void Player::removePlayer() {
  if (!is_alive) {
    throw invalid_argument("Player is already dead");
  }
  is_alive = false;
}

void Player::addCoins(int amount) {
  if (amount < 0) {
    throw invalid_argument("Amount must be positive");
  }
  coins += amount;
}

void Player::removeCoins(int amount) {
  if (amount < 0) {
    throw invalid_argument("Amount must be positive");
  }
  if (amount > coins) {
    throw invalid_argument("Amount must be less than or equal to coins");
  }
  coins -= amount;
}

void Player::gather() {}