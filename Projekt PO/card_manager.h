#ifndef CONSOLECARDMANAGER_CARD_MANAGER_H
#define CONSOLECARDMANAGER_CARD_MANAGER_H

#include <string>
#include <deque>
#include <chrono>
#include "card.h"
#include "handlers.h"
#include "interface.h"

class CardManager
{
private:
    CardManagerUserInterface *userInterfacePtr;
    FileHandler *fileHandlerPtr;
    std::deque<Card> cardDeque;

public:
    //*** getters ***//
    std::deque<Card> getCardDeque() { return cardDeque; };
    //*** operations ***//
    void get_command();
    bool load_card_list_from_file();
    bool add_new_card();
    bool remove_card_by_id();
    bool remove_all_expired_cards();
    bool remove_all_cards();
    //*** utils ***//
    static void get_current_month_and_year(unsigned int& currentMonth, unsigned int& currentYear);
    bool is_expiry_date_current(Card card);
    bool is_expiry_date_current(Card card, unsigned int currentMonth, unsigned int currentYear);
    bool is_expiry_date_current(unsigned int currentMonth, unsigned int currentYear,
                                unsigned int month, unsigned int year);
    bool is_card_number_valid(Card card, std::string& errorMsg);
    static bool is_card_number_valid(std::string& errorMsg, std::string cardNumber);
    //*** constructor ***//
    CardManager(FileHandler *fh, CardManagerUserInterface *ui) : fileHandlerPtr(fh), userInterfacePtr(ui)
    { std::deque<Card> cd; cardDeque = cd; };
    //*** destructor ***//
    ~CardManager() { cardDeque.clear(); };

};

#endif //CONSOLECARDMANAGER_CARD_MANAGER_H