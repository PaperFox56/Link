#ifndef LINK_H
#define LINK_H

/*
 * Header of the link system, this library allow
 * to easily communicate with a CyberRealm virtual world
*/

#include <mutex>
#include <thread>
#include <string>
#include <iostream>
#include <unordered_map>
#include <libsocket/inetserverstream.hpp>
#include <libsocket/inetclientstream.hpp>
#include <libsocket/select.hpp>
#include "packet.h"

namespace Link {

enum Type { UNDEFINED, CLIENT, SERVER };

/*
 * The <Link> class can be used with both server and client sockets (but not at the same time)
 * that's why it contains connections for both servers and clients
 */
class link {
public:
    link();
    link(PacketListener *_dest, uint id);
    void config(PacketListener *_dest, uint _id);
    ~link();
    bool startAsServer(const std::string& host, const std::string& port);
    bool startAsClient(const std::string& host, const std::string& port);
    void send(const Packet& packet);
    bool checkConnection(uint id);
    void stop();

private:
    void runClient(uint id);

    uint id;
    bool active;
    std::string host;
    std::string port;
    std::mutex mtx;
    Type type = UNDEFINED;
    PacketListener *dest;
    libsocket::inet_stream client;
    libsocket::inet_stream_server server;
/* NOTE (if your are not me you don't need to read)
 * please don't change anything below if there is not good reason */
    std::thread* mainThread;
    std::unordered_map<uint, std::thread*> receptionThreads;
    std::unordered_map<uint, libsocket::inet_stream*> sockets;
};
}


#endif
