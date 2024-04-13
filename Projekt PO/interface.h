#ifndef CONSOLECARDMANAGER_INTERFACE_H
#define CONSOLECARDMANAGER_INTERFACE_H

#include <deque>
#include "card.h"

enum userInterfaceCommandEnum {
    UI_COMMAND_EXIT = 0,
    UI_COMMAND_VIEW = 1,
    UI_COMMAND_ADD = 2,
    UI_COMMAND_REMOVE_SINGLE = 3,
    UI_COMMAND_REMOVE_EXPIRED = 4,
    UI_COMMNAD_REMOVE_ALL = 5
};

class CardManagerUserInterface
{
public:
    virtual void get_command(userInterfaceCommandEnum &command) = 0;
    virtual void print_card_deque(std::deque<Card> cardDeque) = 0;
    virtual void print_message(std::string message) = 0;
    virtual void get_from_the_user_u_int(unsigned int& number, std::string printout) = 0;
    virtual void get_from_the_user_string(std::string& text, std::string printout) = 0;
    //*** destructor ***//
    virtual ~CardManagerUserInterface() {};
};

class CardManagerUICommandLine : public CardManagerUserInterface {
public:
    void get_command(userInterfaceCommandEnum &command);
    void print_card_deque(std::deque<Card> cardDeque);
    void print_message(std::string message);
    void get_from_the_user_u_int(unsigned int &number, std::string printout);
    void get_from_the_user_string(std::string &text, std::string printout);
    //*** constructor ***//
    CardManagerUICommandLine() {};
    //*** destructor ***//
    ~CardManagerUICommandLine() {};
};
#endif //CONSOLECARDMANAGER_INTERFACE_H