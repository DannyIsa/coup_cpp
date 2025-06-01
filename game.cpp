#include "game.hpp"
#include "player.hpp"

Game::Game() : playerTurn(nullptr), remainingActions(1) {}

Player &Game::turn() {
  if (playerTurn == nullptr) {
    throw invalid_argument("No player turn");
  }
  return *playerTurn;
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

void Game::addPlayer(Player &player) {
  if (player.isInGame()) {
    throw invalid_argument("Player is not in this game");
  }
  if (playerTurn == nullptr) {
    playerTurn = &player;
  }
  allPlayers.push_back(&player);
}

void Game::removePlayer(Player &player) {
  allPlayers.erase(find(allPlayers.begin(), allPlayers.end(), &player));
}

void Game::validatePlayer(Player &player) {
  validatePlayerNotNull(player);
  validatePlayerAlive(player);
  validatePlayerTurn(player);
  validatePlayerHasActions(player);
}

void Game::validateTarget(Player &target) { validatePlayerAlive(target); }

void Game::validatePlayerNotNull(Player &player) {
  if (&player == nullptr) {
    throw invalid_argument("Player cannot be null");
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

void Game::validatePlayerHasActions(Player &player) {
  if (remainingActions <= 0) {
    throw invalid_argument("No actions remaining in this turn");
  }
}

void Game::performAction(Player &player, ActionType action, Player *target) {
  // Common validation for all actions
  validatePlayer(player);

  if (target) {
    validateTarget(*target);
  }

  switch (action) {
  case ActionType::GATHER:
    executeGather(player);
    break;
  case ActionType::TAX:
    executeTax(player);
    break;
  case ActionType::BRIBE:
    executeBribe(player);
    remainingActions += 2;
    break;
  case ActionType::ARREST:
    if (!target)
      throw invalid_argument("Target required for arrest");
    validateTargetInGame(*target);
    executeArrest(player, *target);
    break;
  case ActionType::SANCTION:
    if (!target)
      throw invalid_argument("Target required for sanction");
    validateTargetInGame(*target);
    executeSanction(player, *target);
    break;
  case ActionType::COUP:
    if (!target)
      throw invalid_argument("Target required for coup");
    validateTargetInGame(*target);
    executeCoup(player, *target);
    break;
  default:
    throw invalid_argument("Invalid action type");
  }

  // Consume one action
  remainingActions--;

  // If no actions left, automatically end turn
  if (remainingActions <= 0) {
    nextTurn();
  }
}

void Game::executeGather(Player &player) { player.addCoins(1); }

void Game::executeTax(Player &player) { player.addCoins(2); }

void Game::executeBribe(Player &player) {
  if (player.getCoins() < 4) {
    throw invalid_argument("Player has not enough coins");
  }
  player.removeCoins(4);
}

void Game::executeArrest(Player &player, Player &target) {
  if (target.getCoins() < 1) {
    throw invalid_argument("Target has no coins to arrest");
  }
  target.removeCoins(1);
  player.addCoins(1);
}

void Game::executeSanction(Player &player, Player &target) {
  if (player.getCoins() < 3) {
    throw invalid_argument("Player has not enough coins");
  }
  player.removeCoins(3);
  // Apply sanction effect to target
}

void Game::executeCoup(Player &player, Player &target) {
  if (player.getCoins() < 7) {
    throw invalid_argument("Player has not enough coins");
  }
  player.removeCoins(7);
  target.removePlayer();
}

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

    if ((*currentIt)->isAlive()) {
      // Clear last action of previous player
      if (playerTurn) {
        playerTurn->clearLastAction();
      }
      playerTurn = *currentIt;
      remainingActions = 1; // Reset actions for new turn
      return;
    }
  } while (*currentIt != startingPlayer);

  // If we got back to starting player, game is finished
  throw invalid_argument("Game is finished - no other alive players");
}
