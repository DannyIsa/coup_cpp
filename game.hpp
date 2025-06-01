#pragma once

#include <string>
#include <vector>
using namespace std;

class Player;

enum class ActionType { NONE, GATHER, TAX, BRIBE, ARREST, SANCTION, COUP };

class Game {
protected:
  vector<Player *> allPlayers;
  Player *playerTurn;
  int remainingActions; // Number of actions left in current turn

public:
  Game();
  ~Game() = default;

  Player &turn();
  vector<string> players();
  void addPlayer(Player &player);
  void removePlayer(Player &player);

  void performAction(Player &player, ActionType action,
                     Player *target = nullptr);

  void nextTurn();

private:
  // Validations
  void validatePlayer(Player &player);
  void validateTarget(Player &target);
  void validatePlayerTurn(Player &player);
  void validateTargetInGame(Player &target);
  void validatePlayerAlive(Player &player);
  void validatePlayerNotNull(Player &player);
  void validatePlayerHasActions(Player &player);

  // Action implementations
  void executeGather(Player &player);
  void executeTax(Player &player);
  void executeBribe(Player &player);
  void executeArrest(Player &player, Player &target);
  void executeSanction(Player &player, Player &target);
  void executeCoup(Player &player, Player &target);
};