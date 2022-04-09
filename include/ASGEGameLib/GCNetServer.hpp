//
// Created by huxy on 23/02/2020.
//

#ifndef GAMELIB_GCNETSERVER_HPP
#define GAMELIB_GCNETSERVER_HPP

#include "GComponent.hpp"
#include <atomic>
#include <kissnet.hpp>
#include <list>
#include <mutex>

class GCNetServer final : public GameComponent
{
 public:
  GCNetServer();
  ~GCNetServer() override;
  void update(double dt) override;
  void start();

  void listen(kissnet::tcp_socket& socket);
  void send(
    const kissnet::buffer<16384>& buffer, size_t length,
    const std::list<std::reference_wrapper<const kissnet::tcp_socket>>& exclude);

  GCNetServer(const GCNetServer&) = delete;
  GCNetServer& operator=(const GCNetServer&) = delete;

 private:
  std::mutex mutex;
  [[maybe_unused]] std::vector<std::thread> workers;
  kissnet::tcp_socket server;
  std::list<kissnet::tcp_socket> connections;
  std::atomic<bool> accept_connections = false;
};

#endif // GAMELIB_GCNETSERVER_HPP
