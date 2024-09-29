#ifndef REQUEST_H
#define REQUEST_H

#define ERROR 0
#define SYN 1
#define ACK 2
#define GET 3
#define END_CON 255

#include "cypher.hpp"

#include <queue>
#include <list>

namespace Link {


// some constants
enum DataType : uchar {
    NONE   = 0b00000000,
    INT    = 0b00000001,
    FLOAT  = 0b00000010,
    STRING = 0b00000100,
    BYTE   = 0b00001000,
    PACKET = 0b00010000,

    ARRAY  = 0b10000000
};

/* Do I really have to explain anything here ?
 * Ok I'll do
 * <origin> and <target> stands for two distincs machines
 * <sender> and <recever> stands for two process (on the same machine or not)
 *
 * I chose unsigned int for the formers because the number of interfaces and virtual machines could
 * beacome huge in no time, but seriously who would run more than 65536 process on the
 * same single machine ?
 * <data> is a void pointer to whatever should be send as data between process
 */
struct Packet {
    uint origin;
    uint target;

    ushort sender;
    ushort recever;

    uchar opcode;

    ushort size; // The size of the data stream in bytes
    uchar dataType;
    void *data;

    uint64_t hash;

    void encrypt(Packet *packet);
};

class PacketListener {
public:
    void put(Packet packet);
    Packet pop();
    bool isEmpty();

private:
    std::queue<Packet, std::list<Packet>> packets;
};
}

#endif
