// #include "../headers/cypher.hpp"
// #include <iostream>
//
// union data64 {
//     uint64_t value;
//     char slot[8];
// };
//
// int main(void) {
//
//     std::string data = "Hello my friend";
//     std::string out;
//     data64 buffer;
//
//     Cypher::Key privateKey;
//     Cypher::Key publicKey;
//
//     Cypher::genKeys(&privateKey, &publicKey);
//
//     std::cout << "N: " << publicKey.n << std::endl;
//     std::cout << "PublicKey: " << publicKey.e << std::endl;
//     std::cout << "PrivateKey: " << privateKey.e << std::endl << std::endl;
//
//     for (int i = 0; i < data.size(); i+=4) {
//         buffer.value = 0;
//         for (int j = 0; j < 4; j++) {
//             buffer.slot[j] = data[i+j];
//         }
//
//         std::cout << buffer.value << std::endl;
//         buffer.value = Cypher::encryptOrDecrypt(buffer.value, publicKey);
//         buffer.value = Cypher::encryptOrDecrypt(buffer.value, privateKey);
//         std::cout << buffer.value << std::endl;
//
//         for (int j = 0; j < 4; j++) {
//             out += buffer.slot[j];
//         }
//     }
//
//     for (int i = 0; i < data.size(); i++) {
//         std::cout << (int)data[i] << " " << (char)data[i] << ((i + 1) % 4 ? "\t" : "\n");
//     }
//     std::cout << std::endl;
//     std::cout << std::endl;
//
//     for (int i = 0; i < out.size(); i++) {
//         std::cout << (int)out[i] << " " << (char)out[i] << ((i + 1) % 4 ? "\t" : "\n");
//     }
//
//     return 0;
// }


int main(void) {

    unsigned int a, b, c;
    c = a / b;

    return 0;
}









