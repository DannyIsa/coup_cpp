#pragma once

#include "game.hpp"
#include <string>

using namespace std;

class Player {
protected:
  Game &game;
  string name;
  int coins;
  bool is_alive;
  ActionType lastAction;

public:
  Player(Game &game, string name);
  virtual ~Player() = default;

  int getCoins();
  bool isAlive();
  string getName();
  void removePlayer();
  void addCoins(int amount);
  void removeCoins(int amount);
  bool isInGame();

  // Action tracking
  ActionType getLastAction() const;
  void clearLastAction();

  virtual void gather();
  virtual void tax();
  virtual void bribe();
  virtual void arrest(Player &target);
  virtual void sanction(Player &target);
  virtual void coup(Player &target);
};