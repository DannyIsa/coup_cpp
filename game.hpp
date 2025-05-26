#pragma once

#include <string>
#include <vector>
using namespace std;

class Player;

class Game {
protected:
  vector<Player> allPlayers;
  Player *playerTurn;

public:
  Game();
  ~Game() = default;

  Player &turn();
  vector<string> players();
  void addPlayer(Player &player);
  void removePlayer(Player &player);
};