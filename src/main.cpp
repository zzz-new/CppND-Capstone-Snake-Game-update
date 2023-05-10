#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"

#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>

struct Score {
  std::string name;
  std::string difficulty;
  int score;
  std::time_t date;
};

void SaveScore(const Score& score) {
  // open file
  std::ofstream file("../score_record.txt", std::ios_base::app);
  if (!file.is_open()) {
    std::cerr << "Failed to open score_record.txt" << std::endl;
    return;
  }

  // write score to file
  std::tm* timeinfo = std::localtime(&score.date);
  file << score.name << "," << score.difficulty << "," << score.score << ","
       << std::put_time(timeinfo, "%Y-%m-%d %H:%M:%S") << std::endl;

  // close file
  file.close();
}


int main() {
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};

  std::string player_name;
  std::cout << "Enter player name: ";
  std::cin >> player_name;

  std::cout << "Select difficulty level: " << std::endl;
  std::cout << "1. Easy" << std::endl;
  std::cout << "2. Medium" << std::endl;
  std::cout << "3. Hard" << std::endl;

  int difficultyLevel;
  std::string difficultyLevelstr;
  std::cin >> difficultyLevel;

  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  Controller controller;
  Game game(kGridWidth, kGridHeight);

  switch(difficultyLevel) {
    case 1:
        game.SetDifficulty(Game::Difficulty::Easy);
        difficultyLevelstr="Easy";
        break;
    case 2:
        game.SetDifficulty(Game::Difficulty::Medium);
        difficultyLevelstr="Medium";
        break;
    case 3:
        game.SetDifficulty(Game::Difficulty::Hard);
        difficultyLevelstr="Hard";
        break;
    default:
        std::cout << "Invalid input. Please select a valid difficulty level." << std::endl;
        // prompt user to select difficulty level again
        break;
  }

  game.Run(controller, renderer, kMsPerFrame);

  std::cout << "Game has terminated successfully!\n";
  std::cout << "Level: " << difficultyLevelstr << "\n";
  std::cout << "Score: " << game.GetScore() << "\n";
  std::cout << "Size: " << game.GetSize() << "\n";

  Score score = {player_name, difficultyLevelstr, game.GetScore(), std::time(nullptr)};
  SaveScore(score);

  return 0;
}