#include "game.hpp"
#include "governor.hpp"
#include "player.hpp"

Game::Game() : playerTurn(nullptr), remainingActions(1) {}

string Game::turn() {
  if (playerTurn == nullptr) {
    throw invalid_argument("No player turn");
  }
  return playerTurn->getName();
}

vector<string> Game::players() {
  vector<string> names;
  for (Player *player : allPlayers) {
    if (player->isAlive()) {
      names.push_back(player->getName());
    }
  }
  return names;
}

string Game::winner() {
  if (allPlayers.size() == 1) {
    return allPlayers[0]->getName();
  }
  throw invalid_argument("No winner");
}

void Game::addPlayer(Player &player) {
  if (playerTurn == nullptr) {
    playerTurn = &player;
  }
  allPlayers.push_back(&player);
}

void Game::removePlayer(Player &player) {
  allPlayers.erase(find(allPlayers.begin(), allPlayers.end(), &player));
}

void Game::validatePlayer(Player &player, ActionType action, int price) {
  validatePlayerAlive(player);
  validatePlayerTurn(player);
  validatePlayerHasActions();
  validatePlayerHasEnoughCoins(player, price);
  validatePlayerHasLessThan10Coins(player, action);
  validatePlayerIsntSanctioned(player, action);
  validateCanArrest(player, action);
}

void Game::validateTarget(Player &target) {
  validatePlayerAlive(target);
  validateTargetInGame(target);
}

void Game::validateCanArrest(Player &player, ActionType action) {
  if (player.isArrestPrevented() && action == ActionType::ARREST) {
    throw invalid_argument("Player cannot perform arrest");
  }
}

void Game::validatePlayerIsntSanctioned(Player &player, ActionType action) {
  if (player.isSanctioned() &&
      (action == ActionType::GATHER || action == ActionType::TAX)) {
    throw invalid_argument("Player is sanctioned");
  }
}

void Game::validatePlayerHasEnoughCoins(Player &player, int price) {
  if (player.getCoins() < price) {
    throw invalid_argument("Player does not have enough coins");
  }
}

void Game::validatePlayerHasLessThan10Coins(Player &player, ActionType action) {
  if (player.getCoins() >= 10 && action != ActionType::COUP) {
    throw invalid_argument("Player has more than 10 coins, must perform coup");
  }
}

void Game::validatePlayerTurn(Player &player) {
  if (playerTurn != &player) {
    throw invalid_argument("It's not your turn");
  }
}

void Game::validateTargetInGame(Player &target) {
  bool targetInGame =
      find(allPlayers.begin(), allPlayers.end(), &target) != allPlayers.end();

  if (!targetInGame) {
    throw invalid_argument("Target player must be in the same game");
  }
}

void Game::validatePlayerAlive(Player &player) {
  if (!player.isAlive()) {
    throw invalid_argument("Dead players cannot perform actions");
  }
}

void Game::validatePlayerHasActions() {
  if (remainingActions <= 0) {
    throw invalid_argument("No actions remaining in this turn");
  }
}

void Game::consumeAction() {
  remainingActions--;

  // If no actions left, automatically end turn
  if (remainingActions <= 0) {
    nextTurn();
  }
}

void Game::resetPlayer(Player &player) {
  player.setSanctioned(false);
  player.setArrestPrevented(false);
  remainingActions = 1;
}

void Game::addActions(int amount) { remainingActions += amount; }

void Game::nextTurn() {
  if (allPlayers.empty())
    return;

  // Find current player position
  auto currentIt = find(allPlayers.begin(), allPlayers.end(), playerTurn);
  if (currentIt == allPlayers.end()) {
    throw invalid_argument("Current player not found in game");
  }

  Player *startingPlayer = playerTurn;

  // Loop through players to find next alive one
  do {
    currentIt++;
    if (currentIt == allPlayers.end()) {
      currentIt = allPlayers.begin();
    }

    playerTurn = *currentIt;
    resetPlayer(*playerTurn);
    return;
  } while (*currentIt != startingPlayer);

  // If we got back to starting player, game is finished
  throw invalid_argument("Game is finished - no other alive players");
}
