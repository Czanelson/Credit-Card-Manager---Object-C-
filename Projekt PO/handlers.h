#ifndef CONSOLECARDMANAGER_HANDLERS_H
#define CONSOLECARDMANAGER_HANDLERS_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>
#include <deque>
#include "card.h"
#include "json.hpp"
#include "plusaes.hpp"

using namespace NLOHMANN_JSON_NAMESPACE;
class EncryptionHandler
{
private:
    const std::vector<unsigned char> key = plusaes::key_from_string(&"E7HnypcQWQrGjNMM"); //16 char = 128-bit
    const unsigned char iv[16] = {
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    };
public:
    std::vector<unsigned char> encrypt(std::string input);
    std::vector<unsigned char> decrypt(std::vector<unsigned char> input);

};

class FileHandler
{
private:
    std::string file;
    EncryptionHandler * encryptionHandlerPtr;
public:
    //*** constructor ***//
    FileHandler(std::string f, EncryptionHandler *eh) : file(f), encryptionHandlerPtr(eh) {};
    //*** destructor ***//
    ~FileHandler() {};
    const void encrypt_and_convert_to_json(json& j, Card card);
    bool write_to_file(std::deque<Card> cards);
    bool read_from_file_and_decrypt(std::deque<Card>& cards, std::string& errorMsg);
};

#endif //CONSOLECARDMANAGER_HANDLERS_H