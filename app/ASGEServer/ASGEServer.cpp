//
// Created by james on 28/01/2021.
//

#include "ASGEServer.hpp"
#include <iostream>

bool ASGEServer::init()
{
  net_server.start();
  return true;
}

int ASGEServer::run()
{
  std::string input;
  while (input != "quit" && input != "exit")
  {
    std::getline(std::cin, input);
  }

  return EXIT_SUCCESS;
}
