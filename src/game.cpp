// src/game.cpp

#include "game.h"
#include <algorithm>
#include <iostream>
#include <random>

constexpr int MIN_PLAYERS = 2;

Game::Game() : board_(3) {}

void Game::init(int playerCount) {
  if (playerCount < MIN_PLAYERS)
    playerCount = MIN_PLAYERS;

  players_ = generateRandomSymbols(playerCount);
  currentPlayer_ = 0;

  board_.resize(boardSize(players_.size()));
}

std::size_t Game::boardSize(std::size_t numPlayers) { return numPlayers + 1; }

bool Game::isBadBoardChar(char ch) {
  return ch == '|' || ch == '+' || ch == ' ' || ch == '-';
}

std::vector<char> Game::generateRandomSymbols(int count) {
  std::vector<char> result;
  result.reserve(count);

  if (count >= 1)
    result.push_back('X');
  if (count >= 2)
    result.push_back('O');
  if ((int)result.size() >= count)
    return result;

  std::vector<char> pool;
  for (int c = 33; c <= 126; ++c) {
    char ch = static_cast<char>(c);
    if (isBadBoardChar(ch))
      continue;
    if (ch == 'X' || ch == 'O')
      continue;
    pool.push_back(ch);
  }

  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::shuffle(pool.begin(), pool.end(), gen);

  for (char ch : pool) {
    if ((int)result.size() >= count)
      break;
    result.push_back(ch);
  }

  return result;
}

bool Game::placeMove(int x, int y, char player) {
  if (x < 0 || y < 0)
    return false;
  if (x >= (int)board_.getSize() || y >= (int)board_.getSize())
    return false;
  if (board_.get(x, y) != ' ')
    return false;
  board_.set(x, y, player);
  return true;
}

char Game::checkWinner() const {
  std::size_t size = board_.getSize();

  // rows
  for (std::size_t y = 0; y < size; ++y) {
    char first = board_.get(0, y);
    if (first == ' ')
      continue;
    bool allSame = true;
    for (std::size_t x = 1; x < size; ++x)
      if (board_.get(x, y) != first) {
        allSame = false;
        break;
      }
    if (allSame)
      return first;
  }

  // cols
  for (std::size_t x = 0; x < size; ++x) {
    char first = board_.get(x, 0);
    if (first == ' ')
      continue;
    bool allSame = true;
    for (std::size_t y = 1; y < size; ++y)
      if (board_.get(x, y) != first) {
        allSame = false;
        break;
      }
    if (allSame)
      return first;
  }

  // main diag
  {
    char first = board_.get(0, 0);
    if (first != ' ') {
      bool allSame = true;
      for (std::size_t i = 1; i < size; ++i)
        if (board_.get(i, i) != first) {
          allSame = false;
          break;
        }
      if (allSame)
        return first;
    }
  }

  // anti diag
  {
    char first = board_.get(size - 1, 0);
    if (first != ' ') {
      bool allSame = true;
      for (std::size_t i = 1; i < size; ++i)
        if (board_.get(size - 1 - i, i) != first) {
          allSame = false;
          break;
        }
      if (allSame)
        return first;
    }
  }

  return ' ';
}

bool Game::playTurn() {
  board_.print();

  char current = players_[currentPlayer_];
  std::cout << "Player " << current << " move (x y): ";

  int x, y;
  if (!(std::cin >> x >> y)) {
    std::cout << "Input ended.\n";
    return false;
  }

  if (!placeMove(x, y, current)) {
    std::cout << "Invalid move, try again.\n";
    return true;
  }

  char winner = checkWinner();
  if (winner != ' ') {
    board_.print();
    std::cout << "Player " << winner << " wins!\n";
    return false;
  }

  if (board_.isFull()) {
    board_.print();
    std::cout << "It's a draw.\n";
    return false;
  }

  currentPlayer_ = (currentPlayer_ + 1) % players_.size();
  return true;
}
