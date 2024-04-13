#ifndef CONSOLECARDMANAGER_CARD_H
#define CONSOLECARDMANAGER_CARD_H

#include <string>

class Card
{
private:
    std::string cardNumber;
    std::string firstName;
    std::string lastName;
    unsigned int month;
    unsigned int year;
    unsigned int cardId;
public:
    //*** getters ***//
    std::string get_card_number() { return cardNumber; }
    const std::string get_first_name() { return firstName; }
    const std::string get_last_name() { return lastName; }
    const unsigned int get_month() { return month; }
    const unsigned int get_year() { return year; }
    const unsigned int get_card_id() { return cardId; }

    //*** constructor ***//
    Card(std::string  n, unsigned int m, unsigned int y, std::string f, std::string l) :
            cardNumber(n), month(m), year(y), firstName(f), lastName(l) {
        cardId = stoul(cardNumber.substr(cardNumber.size() - 4));
    };
    //*** destructor ***///
    ~Card() {};
};

#endif //CONSOLECARDMANAGER_CARD_H
