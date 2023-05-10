#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  enum class Difficulty { Easy, Medium, Hard }; // Define the Difficulty enum
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  void SetDifficulty(Difficulty difficulty);
  int GetScore() const;
  int GetSize() const;
  auto GetDifficulty() const;

 private:
  Snake snake;
  SDL_Point food;
  SDL_Point obstacle;
  std::vector<SDL_Point> obstacles;

  Difficulty difficulty; //{Difficulty::Easy};
  float speed_increase {0};
  int num_obstacles {1};

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};

  void PlaceFood();
  void PlaceObstacle();
  bool ObstacleCell(int x, int y) const;
  void Update();
};

#endif