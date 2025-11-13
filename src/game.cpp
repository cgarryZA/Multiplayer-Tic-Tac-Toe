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
  return generateRandomSymbols(count, {});
}

std::vector<char>
Game::generateRandomSymbols(int count, const std::vector<char> &extraPool) {
  std::vector<char> result;
  result.reserve(count);

  if (count >= 1)
    result.push_back('X');
  if (count >= 2)
    result.push_back('O');
  if ((int)result.size() >= count)
    return result;

  std::vector<char> pool;

  auto push_if_ok = [&](char ch) {
    if (isBadBoardChar(ch))
      return;
    if (ch == 'X' || ch == 'O')
      return;
    if (std::isalpha(static_cast<unsigned char>(ch))) {
      pool.push_back(ch);
    }
  };

  for (char ch : extraPool)
    push_if_ok(ch);

  for (char ch = 'A'; ch <= 'Z'; ++ch)
    push_if_ok(ch);
  for (char ch = 'a'; ch <= 'z'; ++ch)
    push_if_ok(ch);

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
    char c0 = board_.get(0, y);
    int t0 = teamIndexOf(c0);
    if (t0 == -1)
      continue;
    bool allSame = true;
    for (std::size_t x = 1; x < size; ++x) {
      if (teamIndexOf(board_.get(x, y)) != t0) {
        allSame = false;
        break;
      }
    }
    if (allSame)
      return c0;
  }

  for (std::size_t x = 0; x < size; ++x) {
    char c0 = board_.get(x, 0);
    int t0 = teamIndexOf(c0);
    if (t0 == -1)
      continue;
    bool allSame = true;
    for (std::size_t y = 1; y < size; ++y) {
      if (teamIndexOf(board_.get(x, y)) != t0) {
        allSame = false;
        break;
      }
    }
    if (allSame)
      return c0;
  }

  {
    char c0 = board_.get(0, 0);
    int t0 = teamIndexOf(c0);
    if (t0 != -1) {
      bool allSame = true;
      for (std::size_t i = 1; i < size; ++i) {
        if (teamIndexOf(board_.get(i, i)) != t0) {
          allSame = false;
          break;
        }
      }
      if (allSame)
        return c0;
    }
  }

  {
    char c0 = board_.get(size - 1, 0);
    int t0 = teamIndexOf(c0);
    if (t0 != -1) {
      bool allSame = true;
      for (std::size_t i = 1; i < size; ++i) {
        if (teamIndexOf(board_.get(size - 1 - i, i)) != t0) {
          allSame = false;
          break;
        }
      }
      if (allSame)
        return c0;
    }
  }
  return ' ';
}

bool Game::playTurn() {
  board_.print();

  char current = nextScheduledPlayer();
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
    if (teamsMode_) {
      int tid = teamIndexOf(winner);
      std::cout << "Team " << tid << " wins \n";
    } else {
      std::cout << "Player " << winner << " wins!\n";
    }
    return false;
  }

  if (board_.isFull()) {
    board_.print();
    std::cout << "It's a draw.\n";
    return false;
  }

  return true;
}

void Game::initWithPlayers(const std::vector<char> &players) {
  players_ = players;
  currentPlayer_ = 0;
  board_.resize(boardSize(players_.size()));

  teams_.clear();
  teamsMode_ = false;
  currentTeam_ = inChunkUsed_ = 0;
  chunkSize_ = 1;
}

void Game::enableTeams(const std::vector<std::vector<char>> &teams) {
  std::vector<char> flat;
  for (auto &t : teams)
    flat.insert(flat.end(), t.begin(), t.end());

  auto has = [&](char c) {
    return std::find(players_.begin(), players_.end(), c) != players_.end();
  };
  for (char c : flat) {
    (void)has(c);
  }

  std::size_t mx = 1;
  teams_.clear();
  teams_.reserve(teams.size());
  for (auto &t : teams) {
    Team T;
    T.members = t;
    T.memberIndex = 0;
    teams_.push_back(std::move(T));
    if (t.size() > mx)
      mx = t.size();
  }
  chunkSize_ = mx;
  teamsMode_ = true;
  currentTeam_ = 0;
  inChunkUsed_ = 0;
  playerToTeam_.clear();
  for (std::size_t ti = 0; ti < teams_.size(); ++ti) {
    for (char c : teams_[ti].members) {
      playerToTeam_[c] = ti;
    }
  }
}

char Game::nextScheduledPlayer() {
  if (!teamsMode_) {
    char p = players_[currentPlayer_];
    currentPlayer_ = (currentPlayer_ + 1) % players_.size();
    return p;
  }
  if (inChunkUsed_ >= chunkSize_) {
    inChunkUsed_ = 0;
    currentTeam_ = (currentTeam_ + 1) % teams_.size();
  }
  Team &T = teams_[currentTeam_];
  char player = T.members[T.memberIndex];
  T.memberIndex = (T.memberIndex + 1) % T.members.size();
  ++inChunkUsed_;
  return player;
}

int Game::teamIndexOf(char c) const {
  if (c == ' ')
    return -1;
  if (!teamsMode_)
    return static_cast<int>(c);
  auto it = playerToTeam_.find(c);
  if (it == playerToTeam_.end())
    return static_cast<int>(c);
  return static_cast<int>(it->second);
}
