//
// Created by huxy on 23/02/2020.
//

#ifndef NETGAME_GCNETCLIENT_HPP
#define NETGAME_GCNETCLIENT_HPP

#include "GComponent.hpp"
#include <atomic>
#include <iostream>
#include <kissnet.hpp>
#include <string>
#include <thread>

class GCNetClient : public GameComponent
{
 public:
  GCNetClient();
  ~GCNetClient() override;
  void update(double dt) override;

  void run();
  void input();
  void receive();
  void receivePackets(unsigned long long int i, kissnet::buffer<16384> array);
  void connect(const std::string& server_ip, unsigned short server_port);

  GCNetClient(const GCNetClient&) = delete;
  GCNetClient& operator=(const GCNetClient&) = delete;
  std::string incoming_chat;
  std::vector<std::string> chat;
  kissnet::tcp_socket socket;

 private:
  std::atomic<bool> connected = false;
  std::atomic<bool> running   = false;
};

#endif // NETGAME_GCNETCLIENT_HPP
