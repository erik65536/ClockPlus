#include "Application.h"

int main(int narg, char** args) {
  auto app = ClockPlus::Application::create(narg, args);
  if(!app) {
    return -1;
  }

  return app->run();
}