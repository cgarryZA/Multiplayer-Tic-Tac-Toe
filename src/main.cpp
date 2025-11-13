#include "game.h"
#include <algorithm> // for std::all_of
#include <cctype>
#include <iostream>
#include <limits>
#include <sstream>
#include <unordered_set>

static std::vector<std::vector<char>>
parseTeamsLine(const std::string &line, const std::vector<char> &roster) {
  std::unordered_set<char> rosterSet(roster.begin(), roster.end());

  auto is_all_digits = [](const std::string &s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
  };

  // Split by '|'
  std::vector<std::vector<char>> teams;
  std::istringstream top(line);
  std::string teamChunk;
  while (std::getline(top, teamChunk, '|')) {
    std::istringstream ts(teamChunk);
    std::string tok;
    std::vector<char> team;

    while (ts >> tok) {
      if (tok.size() == 1 &&
          !std::isdigit(static_cast<unsigned char>(tok[0]))) {
        char sym = tok[0];
        if (!rosterSet.count(sym))
          throw std::runtime_error("Unknown symbol: " + tok);
        team.push_back(sym);
      } else if (is_all_digits(tok)) {
        int idx = std::stoi(tok);
        if (idx < 1 || idx > static_cast<int>(roster.size()))
          throw std::runtime_error("Index out of range: " + tok);
        team.push_back(roster[idx - 1]);
      } else {
        throw std::runtime_error("Bad token: " + tok);
      }
    }
    if (!team.empty())
      teams.push_back(std::move(team));
  }
  if (teams.empty())
    throw std::runtime_error("No teams parsed");

  // Validate coverage & duplicates: each roster member must appear exactly once
  std::unordered_set<char> seen;
  for (auto &t : teams)
    for (char c : t) {
      if (!rosterSet.count(c))
        throw std::runtime_error("Unknown player in team");
      if (!seen.insert(c).second)
        throw std::runtime_error("Duplicate player in teams");
    }
  if (seen.size() != roster.size())
    throw std::runtime_error("Some players not assigned to any team");

  if (teams.size() < 2)
    throw std::runtime_error("Must have at least 2 teams");

  return teams;
}

int main() {
  Game game;

  int players;
  std::cout << "Enter number of players: ";
  if (!(std::cin >> players)) {
    std::cout << "Input ended.\n";
    return 0;
  }

  game.init(players);

  // Show roster so users know symbols
  const auto &roster = game.players();
  std::cout << "Players: ";
  for (std::size_t i = 0; i < roster.size(); ++i) {
    std::cout << "[" << (i + 1) << ":" << roster[i] << "] ";
  }
  std::cout << "\nEnable teams? (y/n): ";
  char yn;
  std::cin >> yn;

  if (yn == 'y' || yn == 'Y') {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                    '\n'); // eat EOL
    std::cout << "Enter teams as symbols or indices (space-separated), teams "
                 "separated by '|'\n";
    std::cout << "Examples:\n";
    std::cout << "  X O | A      (2 vs 1)\n";
    std::cout << "  1 2 | 3      (using indices)\n";
    std::cout << "  A B C | D E | F G   (3,2,2)\n";
    std::cout << "> ";

    std::string line;
    std::getline(std::cin, line);

    try {
      auto teams = parseTeamsLine(line, roster);
      game.enableTeams(teams);
      std::cout << "Teams enabled.\n";
    } catch (const std::exception &e) {
      std::cout << "Invalid teams: " << e.what()
                << "\nContinuing without teams.\n";
    }
  }

  while (game.playTurn()) {
    // loop
  }
  return 0;
}
