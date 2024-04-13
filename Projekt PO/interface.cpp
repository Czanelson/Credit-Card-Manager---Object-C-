#include <iostream>
#include "interface.h"

using namespace std;

void CardManagerUICommandLine::get_command(userInterfaceCommandEnum& command)
{
    unsigned int option;
    bool isOptionCorrect;
    do {
        cout << "Please enter the number of the operation you wish to perform: " << endl
             << "0. Exit" << endl
             << "1. View saved cards" << endl
             << "2. Add a new card" << endl
             << "3. Remove card" << endl
             << "4. Remove all expired cards" << endl
             << "5. Remove all cards" << endl;
        cin >> option;
        isOptionCorrect = (option >= 0 && option <= 5);
        if (!isOptionCorrect)
            cout << "Please provdie correct option number (0-5).";
    } while (!isOptionCorrect);
    command = static_cast<userInterfaceCommandEnum>(option);
}

void CardManagerUICommandLine::print_card_deque(deque<Card> cardDeque)
{
    if (cardDeque.empty())
    {
        print_message("Card deque is empty, there is nothing to show!");
        return;
    }
    for (auto it = cardDeque.begin(); it != cardDeque.end(); ++it)
    {
        cout << "Card id: " << it->get_card_id() << endl;
        cout << "Card number: " << it->get_card_number() << endl;
        cout << "Expiration date: " << it->get_month() << "/" << it->get_year() << endl;
        cout << "Owner: " << it->get_first_name() << " " << it->get_last_name() << endl;
        cout << "__________________________________________________________________" << endl;
    }
}

void CardManagerUICommandLine::print_message(string message)
{
    cout << message << endl;
}

void CardManagerUICommandLine::get_from_the_user_u_int(unsigned int& number, string printout)
{
    cout << printout << endl;
    cin >> number;
}

void CardManagerUICommandLine::get_from_the_user_string(string &text, string printout)
{
    cout << printout << endl;
    cin >> text;
}