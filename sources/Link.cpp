#include "../headers/link.h"

using namespace libsocket;

namespace Link {

/**
 * This function is called to manage a spacific connection
 * @param id: the socket id
 */
void link::runClient(uint id) {
    Packet packet;
    inet_stream *conn = sockets[id];
    std::cout << "Link - Client connected, ID: " << id << std::endl;
    while (active && sockets.find(id) != NULL) {
        try {
            // Get the packet
            conn->rcv(&packet.origin, sizeof(uint), MSG_WAITALL);
            conn->rcv(&packet.sender, sizeof(ushort), MSG_WAITALL);
            conn->rcv(&packet.recever, sizeof(ushort), MSG_WAITALL);
            conn->rcv(&packet.size, sizeof(ushort), MSG_WAITALL);
            if (packet.size > 0) {
                conn->rcv(&packet.dataType, sizeof(uchar), MSG_WAITALL);
                /*
                 * we have to allocate memory space to store the data
                 * that also meam that this pointer should be freed  after usage
                 */
                packet.data = malloc(packet.size);
                conn->rcv(packet.data, packet.size, MSG_WAITALL);
            }
            conn->rcv(&packet.opcode, sizeof(uchar), MSG_WAITALL);
            if (packet.opcode != ERROR)
                dest->put(packet);
            else {
                dest->put(Packet({id, this->id, 0, 0, END_CON, 0, NONE, nullptr}));
                break;
            }
        } catch (const libsocket::socket_exception& exc) {
            std::cerr << "Link - Reception: " << exc.mesg << std::endl;
            break;
        }
    }
    mtx.lock();
    std::cout << "Link - Client disconnected, ID: " << id << std::endl;
    sockets.erase(id);
    receptionThreads.erase(id);
    mtx.unlock();
}

link::link() {}

link::link(PacketListener *_dest, uint _id) {
    config(_dest, _id);
}

void link::config(PacketListener *_dest, uint _id) {
    id = _id;
    active = true;
    dest = _dest;
}

bool link::checkConnection(uint id) {
    mtx.lock();
    bool found = sockets.find(id) != NULL;
    mtx.unlock();
    return found;
}

link::~link() { }

/**
 * Start the link as a server (only if it isn't already configured)
 * This function lauch the reception thread that take care of entering connections
 * @param host: the machine ip address
 * @param port: the listerning port
 */
bool link::startAsServer(const std::string& host, const std::string& port) {
    if (type != UNDEFINED) return false;
    try {
        server.setup(host, port, LIBSOCKET_IPv6);
        mainThread = new std::thread([this]() {
            std::cout << "Link - Connection thread ready..." << std::endl;
            active = true;
            inet_stream* conn;
            while (active) {
                uint clientId;
                conn = server.accept(0);
                if (!active) break;
                conn->rcv(&clientId, 4);
                if (sockets.find(clientId) != NULL) {
                    std::cout << "Link - conflit with client ID: " << clientId << std::endl;
                    conn->destroy();
                    continue;
                }
                conn->snd(&id, 4);
                sockets[clientId] = conn;
                receptionThreads[clientId] = new std::thread(&link::runClient, this, clientId);
            }
        });
    } catch (const libsocket::socket_exception& exc) {
        std::cerr << exc.mesg << std::endl;;
        return false;
    }
    type = SERVER;
    std::cout << "Link - Link procedure inited as server..." << std::endl;
    return true;
}

/**
 * Start the link as a client (only if it isn't already configured)
 * This function lauch the reception thread that take care of the connection with a server
 * @param host: the machine ip address
 * @param port: the listerning port
 */
bool link::startAsClient(const std::string& host, const std::string& port) {
    if (type != UNDEFINED) return false;
    try {
        client.connect(host, port, LIBSOCKET_IPv6);
        uint num = 1;
        client.snd(&id, 4);
        client.rcv(&num, 4);
        sockets[num] = &client;
        receptionThreads[num] = new std::thread(&link::runClient, this, num);
        std::cout << "Link - Link procedure inited as client..." << std::endl;
        active = true;
    } catch (const libsocket::socket_exception& exc) {
        std::cerr << exc.mesg << std::endl;
        return false;
    }
    return true;
}

void link::send(const Packet& packet) {
    int id = packet.target;
    if (sockets.find(id) == NULL) return;
    try {
        mtx.lock();
        // We don't have to send the target id, because, you know...
        sockets[id]->snd(&packet.origin, sizeof(uint));
        sockets[id]->snd(&packet.sender, sizeof(ushort));
        sockets[id]->snd(&packet.recever, sizeof(ushort));
        // For data, it's a bit special
        sockets[id]->snd(&packet.size, sizeof(ushort));
        if (packet.size > 0) {
            sockets[id]->snd(&packet.dataType, sizeof(uchar));
            sockets[id]->snd(packet.data, packet.size);
        }
        sockets[id]->snd(&packet.opcode, sizeof(uchar));
    } catch (const libsocket::socket_exception& exc) {
        std::cerr << "Link - send :" << exc.mesg << std::endl;
    }
    mtx.unlock();
}

void link::stop() {
    if (!active) return;
    active = false;
    try {
        mtx.lock();
        for (auto it : sockets) {
            it.second->destroy();
        }
        mtx.unlock();
        if (type == SERVER) {
            server.destroy();
            /* The actual code have an issue, the server continue to wait for connections
            * even after being shut down, this is why the block below simulate an entering connection
            * just to allow the mainThread to stop correctly
            *
            * TODO fix this bullshit
            * WARNING putting SOCK_NONBLOCK in the accept calling wont fix it
            */
            inet_stream temp(server.gethost(), server.getport(), LIBSOCKET_IPv6);
            mainThread->join();
        }
    } catch (const libsocket::socket_exception& exc) {
        std::cerr << "Link - Stop :" << exc.mesg << std::endl;
    }
    std::cout << "Link - Clean process ended" << std::endl;
}
}
