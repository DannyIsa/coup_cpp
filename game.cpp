#include "game.hpp"

Game::Game() : playerTurn(nullptr) {}

Player &Game::turn() {
  if (playerTurn == nullptr) {
    throw invalid_argument("No player turn");
  }
  return *playerTurn;
}

void Game::addPlayer(Player &player) {
  if (playerTurn == nullptr) {
    playerTurn = &player;
  }
  allPlayers.push_back(&player);
}

void Game::removePlayer(Player &player) {
  allPlayers.erase(find(allPlayers.begin(), allPlayers.end(), player));
}
