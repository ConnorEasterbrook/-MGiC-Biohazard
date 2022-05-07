//
// Created by huxy on 23/02/2020.
//

#ifndef NETGAME_GCNETCLIENT_HPP
#define NETGAME_GCNETCLIENT_HPP

#include "../ASGEGameLib/gameObject/battleUnits/BattleUnit.h"
#include "GComponent.hpp"
#include <atomic>
#include <iostream>
#include <kissnet.hpp>
#include <mutex>
#include <string>
#include <thread>

class GCNetClient : public GameComponent {
public:
    GCNetClient();
    ~GCNetClient() override;
    void update(double dt) override;

    void run();
    void input();
    void receive();
    void receivePackets(unsigned long long int i, kissnet::buffer<16384> array);
    void connect(const std::string &server_ip, unsigned short server_port);

    //  void sendString(const std::string& mail);

    GCNetClient(const GCNetClient &) = delete;
    GCNetClient &operator=(const GCNetClient &) = delete;
    std::string incoming_chat;
    std::vector<std::string> chat;
    kissnet::tcp_socket socket;
    int state = 1;

    int lobby = 0;


    int p1_sniper = 0;
    int p2_sniper = 0;
    int p1_medic = 0;
    int p2_medic = 0;
    int unit_selected = 0;

    int unit_count = 0;

    int unit_count_co = 0;
    bool in_game1 = false;
    bool in_game2 = false;

    BattleUnit units[8];
    bool player2 = false;
    bool turn_over = false;


private:
    std::mutex mutex;
    std::atomic<bool> connected = false;
    std::atomic<bool> running = false;
};

#endif// NETGAME_GCNETCLIENT_HPP
