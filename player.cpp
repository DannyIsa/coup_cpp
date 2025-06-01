#include "player.hpp"

Player::Player(Game &game, string name)
    : game(game), name(name), coins(0), is_alive(true),
      lastAction(ActionType::NONE) {
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

bool Player::isInGame() { return &game != nullptr; }

ActionType Player::getLastAction() const { return lastAction; }

void Player::clearLastAction() { lastAction = ActionType::NONE; }

void Player::gather() {
  game.performAction(*this, ActionType::GATHER);
  lastAction = ActionType::GATHER;
}
void Player::tax() {
  game.performAction(*this, ActionType::TAX);
  lastAction = ActionType::TAX;
}
void Player::bribe() {
  game.performAction(*this, ActionType::BRIBE);
  lastAction = ActionType::BRIBE;
}
void Player::arrest(Player &target) {
  if (&target == nullptr) {
    throw invalid_argument("Target cannot be null");
  }
  game.performAction(*this, ActionType::ARREST, &target);
  lastAction = ActionType::ARREST;
}
void Player::sanction(Player &target) {
  if (&target == nullptr) {
    throw invalid_argument("Target cannot be null");
  }
  game.performAction(*this, ActionType::SANCTION, &target);
  lastAction = ActionType::SANCTION;
}
void Player::coup(Player &target) {
  if (&target == nullptr) {
    throw invalid_argument("Target cannot be null");
  }
  game.performAction(*this, ActionType::COUP, &target);
  lastAction = ActionType::COUP;
}