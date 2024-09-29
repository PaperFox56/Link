#include "headers/link.h"

using libsocket::inet_stream;
using libsocket::unique_ptr;

int main(void) {
    Link::PacketListener list;
    Link::link myLink(&list, 4);

    std::string host = "::1";
    std::string port = "1138";
    u_int8_t buffer[100] = {0};
/*
    try {
        if (!myLink.startAsServer(host, port)) return -1;

        int i = 0;
        Link::Packet packet;
        while (list.isEmpty());
        packet = list.pop();
        std::cout << "Packet received: " << (char*)packet.data << std::endl;
        packet.opcode = 78;
        packet.target = packet.origin;
        ((char*)(packet.data))[0] = '5';
        myLink.send(packet);
        free(packet.data); // WARNING This is an dynamicly allocated pointer so you have to free it
    } catch (const libsocket::socket_exception& exc) {
        std::cerr << exc.mesg << std::endl;;
    }*/
    if (!myLink.startAsServer(host, port)) return -1;
    myLink.stop();

    return 0;
}
