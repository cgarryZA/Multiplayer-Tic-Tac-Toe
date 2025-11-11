// game.hpp
#pragma once
#include "board.h"
#include <vector>

class Game {
public:
  Game();

  void init(int playerCount);
  void initWithPlayers(const std::vector<char>& players);
  void enableTeams(const std::vector<std::vector<char>>& teams);
  char nextScheduledPlayer();
  static std::size_t boardSize(std::size_t numPlayers);
  bool placeMove(int x, int y, char player);
  char checkWinner() const;
  bool playTurn();

private:
  static bool isBadBoardChar(char ch);
  static std::vector<char> generateRandomSymbols(int count);

  struct Team {
    std::vector<char> members;
    std::size_t memberIndex = 0;
  };
  std::vector<Team> teams_;
  std::size_t currentTeam_ = 0;
  std::size_t inChunkUsed_ = 0;
  std::size_t chunkSize_ = 1;
  bool teamsMode_ = false;

  Board board_;
  std::vector<char> players_;
  std::size_t currentPlayer_ = 0;
};
