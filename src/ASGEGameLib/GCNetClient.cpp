//
// Created by huxy on 23/02/2020.
//

#include "GCNetClient.hpp"
#include "sstream"
#include <iostream>
#include <thread>

#include <list>

#include <MyPacket.hpp>
#include <gameObject/battleUnits/BattleUnit.h>
#include <utility>

GCNetClient::GCNetClient() : GameComponent(ID::NETWORK_CLIENT) {}

void GCNetClient::update(double /*dt*/) {
    run();
}

void GCNetClient::connect(const std::string &server_ip, unsigned short server_port) {
    socket = (kissnet::endpoint{server_ip, server_port});
    if (socket.connect()) {
        connected = true;
    } else {
        connected = false;
    }
}

GCNetClient::~GCNetClient() {
    socket.close();
}
void GCNetClient::run() {
    std::thread input_thread([&] { input(); });

    running = true;
    input_thread.detach();

    std::thread forward_thread([&] {
        while (running && connected) {
            receive();
        }
    });

    forward_thread.detach();
}
void GCNetClient::input() {
    while (running) {
        std::string string;
        std::getline(std::cin, string);
        std::cin.clear();
        if (connected) {
            //     std::string str {"msg sent"};
            std::string net_string{std::to_string(string.size()) + ":" + string + ","};
            socket.send(reinterpret_cast<std::byte *>(net_string.data()), net_string.length());
        }
    }
}

void GCNetClient::receive() {

    kissnet::buffer<16384> static_buffer;
    while (connected) {
        if (auto [size, valid] = socket.recv(static_buffer); valid) {
            if (valid.value == kissnet::socket_status::cleanly_disconnected) {
                connected = false;
                std::cout << "clean disconnection" << std::endl;
                socket.close();
                break;
            }

            if (size < static_buffer.size()) {
                static_buffer[size] = std::byte{0};
            }

            receivePackets(size, static_buffer);
        } else {
            connected = false;
            std::cout << "disconnected" << std::endl;
            socket.close();
        }
    }
}

void GCNetClient::receivePackets(unsigned long long int size, kissnet::buffer<16384> static_buffer) {
    MyPacket packet;
    packet.data().resize(size);

    std::copy(
            static_buffer.begin(), static_buffer.begin() + static_cast<ptrdiff_t>(size), &packet.data()[0]);

    auto id = static_cast<MyPacket::PacketID>(0);
    packet >> id;

    //    for (int i = 0; i < packet_amount; ++i)
    {
        //  packet >> id;

        if (id == MyPacket::PacketID::CHAT) {
            packet >> incoming_chat;
            chat.emplace_back(incoming_chat + "\n");
            std::cout << "int = " + incoming_chat << std::endl;
        }
        if (id == MyPacket::PacketID::INT) {
            packet >> lobby;
        }
        if (id == MyPacket::PacketID::STRING) {
            std::string string;
            packet >> string;

            std::cout << string << std::endl;
        }
        if (id == MyPacket::PacketID::BOOL) {
            packet >> player2;
        }
        if (id == MyPacket::PacketID::STATE) {
            packet >> state;
        }
        if (id == MyPacket::PacketID::TEAM2) {
            packet >> p2_medic;
            packet >> p2_sniper;
        }
        if (id == MyPacket::PacketID::TEAM1) {
            packet >> p1_medic;
            packet >> p1_sniper;
        }
        if (id == MyPacket::PacketID::TURN) {
            packet >> turn_over;
            packet >> unit_selected;
        }

        if (id == MyPacket::PacketID::PLAYER) {
            BattleUnit unit2 = BattleUnit();

            packet >> unit_count;

            packet >> unit2;
            units[unit_count].unit_health = unit2.unit_health;
            units[unit_count].visibility = unit2.visibility;
        }
        if (id == MyPacket::PacketID::INGAME1) {
            packet >> in_game1;
        }
        if (id == MyPacket::PacketID::INGAME2) {
            packet >> in_game2;
        }
    }
    std::lock_guard<std::mutex> lck(mutex);
    if (id == MyPacket::PacketID::COORDINATES) {
        packet >> unit_count_co;
        float y;
        float x;
        packet >> x;
        packet >> y;
        units[unit_count_co].xpos = x;
        units[unit_count_co].ypos = y;
    }


    // treat all data as string and append null terminator
    if (size < static_buffer.size()) {
        static_buffer[size] = std::byte{0};
    }

    std::cout << reinterpret_cast<const char *>(static_buffer.data()) << '\n';
}
