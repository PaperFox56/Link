#include "../headers/packet.h"


void Packet::encrypt(Packet *packet) {
    /**
     * WARNING this function not only encrypt the fields of the packet but also the data pointed by it,
     * so take care of keeping a copy of your data
     */

    packet->
}

namespace Link {

void PacketListener::put(Packet packet) {
    packets.push(packet);
}

Packet PacketListener::pop() {
    Packet packet = packets.front();
    packets.pop();
    return packet;
}

bool PacketListener::isEmpty() {
    return packets.empty();
}

}
