#include "headers/link.h"

int main(void) {
    using std::string;

    string host = "::1";
    string port = "1138";

    Link::PacketListener list;
    Link::link myLink(&list, 3);

    u_int8_t buffer[100] = {0};

    if (!myLink.startAsClient(host, port)) return -1;
    char str[] = "g561516";
    Link::Packet packet{3, 4, 0, 0, 45, sizeof(str), STRING, (void*)str};
    myLink.send(packet);
    while (list.isEmpty() && myLink.checkConnection(4));
    if (list.isEmpty()) {
        myLink.stop();
        packet.opcode = 0;
    } else {
        packet = list.pop();
        if (packet.opcode != ERROR) {
            std::cout << "Packet received: " << (char*)packet.data << std::endl;
            free(packet.data); // WARNING This is an dynamicly allocated pointer so you have to free it
        } else {
            return -2;
        }
    }

    return 0;
}
