#include "handlers.h"

std::vector<unsigned char> EncryptionHandler::encrypt(std::string input)
{
    const auto inputSize = plusaes::get_padded_encrypted_size(input.size());
    std::vector<unsigned char> output(inputSize);
    plusaes::encrypt_cbc((unsigned char*)input.data(), input.size(), &key[0], key.size(),
                         &iv, &output[0], output.size(), true);
    return output;
}

std::vector<unsigned char> EncryptionHandler::decrypt(std::vector<unsigned char> input)
{
    unsigned long paddedSize = 0;
    const auto inputSize = plusaes::get_padded_encrypted_size(input.size());
    std::vector<unsigned char> output(inputSize);
    plusaes::decrypt_cbc(&input[0], input.size(), &key[0], key.size(),
                         &iv, &output[0], output.size(), &paddedSize);
    return output;
}

const void FileHandler::encrypt_and_convert_to_json(json& j, Card card)
{
    j = { {"cardNumber", encryptionHandlerPtr->encrypt(card.get_card_number())} };
    j.push_back({ "expirationMonth", encryptionHandlerPtr->encrypt(std::to_string(card.get_month())) });
    j.push_back({ "expirationYear", encryptionHandlerPtr->encrypt(std::to_string(card.get_year())) });
    j.push_back({ "ownerFirstName", encryptionHandlerPtr->encrypt(card.get_first_name()) });
    j.push_back({ "ownerLastName", encryptionHandlerPtr->encrypt(card.get_last_name()) });
}

bool FileHandler::write_to_file(std::deque<Card> cards)
{
    std::ofstream o;
    o.open(file, std::ofstream::out | std::ofstream::trunc);
    std::deque<Card>::iterator it;
    json j;
    o << "{\"cards\":[\n";
    for (it = cards.begin(); it != cards.end(); ++it)
    {
        if (it != cards.begin()) o << ",";
        encrypt_and_convert_to_json(j, *it);
        o << j;
    }
    o << "\n]}";
    o.close();
    return true;
}

bool FileHandler::read_from_file_and_decrypt(std::deque<Card>& cards, std::string& errorMsg)
{
    json j;
    std::ifstream f;
    if (!std::filesystem::exists(file))
    {
        errorMsg = "File not exists!";
        return false;
    }
    f.open(file);
    if (std::filesystem::is_empty(file))
    {
        errorMsg = "File is empty!";
        return false;
    }
    std::stringstream ss;
    ss << f.rdbuf();
    try
    {
        j = json::parse(ss.str());
    }
    catch (json::parse_error& e)
    {
        std::cout << "message: " << e.what() << '\n'
                  << "exception id: " << e.id << '\n'
                  << "byte position of error: " << e.byte << std::endl;
    }
    for (auto jsonCardInstance : j["cards"])
    {
        std::vector<unsigned char> decryptedCardNumber =
                encryptionHandlerPtr->decrypt(jsonCardInstance["cardNumber"]);
        std::vector<unsigned char> decryptedMonth =
                encryptionHandlerPtr->decrypt(jsonCardInstance["expirationMonth"]);
        std::vector<unsigned char> decryptedYear =
                encryptionHandlerPtr->decrypt(jsonCardInstance["expirationYear"]);
        std::vector<unsigned char> decryptedFirstName =
                encryptionHandlerPtr->decrypt(jsonCardInstance["ownerFirstName"]);
        std::vector<unsigned char> decryptedLastName =
                encryptionHandlerPtr->decrypt(jsonCardInstance["ownerLastName"]);

        std::string cardNumber = std::string(decryptedCardNumber.begin(), decryptedCardNumber.end());
        cardNumber = cardNumber.substr(0, 16);
        std::string smonth = std::string(decryptedMonth.begin(), decryptedMonth.end());
        std::string syear = std::string(decryptedYear.begin(), decryptedYear.end());
        unsigned int month = std::stoul(smonth);
        unsigned int year = std::stoul(syear);
        std::string firstName = std::string(decryptedFirstName.begin(), decryptedFirstName.end());
        std::string lastName = std::string(decryptedLastName.begin(), decryptedLastName.end());
        Card card(cardNumber, month, year, firstName, lastName);
        cards.push_back(card);
    }
    f.close();
    return true;
}