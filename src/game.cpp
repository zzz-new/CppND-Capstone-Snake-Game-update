#include "game.h"
#include <iostream>
#include "SDL.h"
//#include "snake.h"

void Game::SetDifficulty(Difficulty difficulty) {
  this->difficulty = difficulty;
  // adjust game settings based on difficulty
  if (difficulty == Difficulty::Easy){
    speed_increase = 0.005;
    num_obstacles = 1;
  } 
  else if (difficulty == Difficulty::Medium){
    speed_increase = 0.02;
    num_obstacles = 3;
  }
  else if (difficulty == Difficulty::Hard){
    speed_increase = 0.1;
    num_obstacles = 5;
  }
}

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {
  //for (int i=0; i<=5; i++){ PlaceObstacle();}
  PlaceFood();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;
  
  for (int i=0; i<num_obstacles; i++){ PlaceObstacle();}

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update();
    // renderer.Render(snake, food, obstacle);
    renderer.Render(snake, food, obstacles);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item
    // nor by an obstacle item before placing food.
    if (!snake.SnakeCell(x, y) && x!=obstacle.x && y!=obstacle.y ) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

/* void Game::PlaceObstacle() {
int x, y;
while (true) {
  x = random_w(engine);
  y = random_h(engine);
  // Check that the location is not occupied by a snake item
  // obstacle.
  if (!snake.SnakeCell(x, y) ) {
    obstacle.x = x;
    obstacle.y = y;
    return;
  }
}
} */

void Game::PlaceObstacle() {
  SDL_Point obstacle;
  while (true) {
    obstacle.x = random_w(engine);
    obstacle.y = random_h(engine);
    // Check if obstacle is already occupied by the snake or other obstacles
    if (!snake.SnakeCell(obstacle.x, obstacle.y) && !ObstacleCell(obstacle.x, obstacle.y)) {
      obstacles.push_back(obstacle);
      break;
    }
  }
}

bool Game::ObstacleCell(int x, int y) const {
  for (auto const &obstacle : obstacles) {
    if (obstacle.x == x && obstacle.y == y) {
      return true;
    }
  }
  return false;
}


void Game::Update() {
  if (!snake.alive) return;

  snake.Update();
  //PlaceObstacle();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    score++;
    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += speed_increase;
    //std::cout << "speed: " << snake.speed << "\n";
  }

  // Check if obstacle is over here
  // if (obstacle.x == new_x && obstacle.y == new_y) {
  //   snake.alive = false;
  for (const auto& obstacle : obstacles) {
    if (obstacle.x == new_x && obstacle.y == new_y) {
        snake.alive = false;
    }
  }

}

auto Game::GetDifficulty() const { return difficulty; }
int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }