#include "card_manager.h"

void CardManager::get_current_month_and_year(unsigned int& currentMonth, unsigned int& currentYear)
{
    const auto now = std::chrono::system_clock::now();
    time_t tt = std::chrono::system_clock::to_time_t(now);
    tm utc_tm;
    gmtime_s(&utc_tm, &tt);
    currentMonth = utc_tm.tm_mon + 1;
    currentYear = utc_tm.tm_year + 1900;
}

bool CardManager::is_card_number_valid(Card card, std::string& errorMsg)
{
    return this->is_card_number_valid(errorMsg, card.get_card_number());
}

bool CardManager::is_expiry_date_current(Card card)
{
    unsigned int currentMonth;
    unsigned int currentYear;
    CardManager::get_current_month_and_year(currentMonth, currentYear);
    return is_expiry_date_current(currentMonth, currentYear, card.get_month(), card.get_year());
}

bool CardManager::is_expiry_date_current(Card card, unsigned int currentMonth, unsigned int currentYear)
{
    return is_expiry_date_current(currentMonth, currentYear, card.get_month(), card.get_year());
}

bool CardManager::is_expiry_date_current(unsigned int currentMonth, unsigned int currentYear,
                                         unsigned int month, unsigned int year)
{
    if (!(month >= 1 && month <= 12))
        return false;
    else if (year<currentYear || year > currentYear + 10)
        return false;
    else if (currentYear < year)
        return true;
    else if (currentYear == year && currentMonth <= month)
        return true;
    else
        return false;
}

bool CardManager::is_card_number_valid(std::string& errorMsg, std::string cardNumber)
{
    const unsigned int cardNumberSize = 16;
    unsigned int controlSum = 0;

    if (cardNumber.size() != cardNumberSize)
    {
        errorMsg = "The card number provided is incorrect. The card number should consist of 16 digits!";
        return false;
    }

    for (int i(0); i < cardNumberSize; ++i)
    {
        int multiplier = (i % 2 == 0) ? 2 : 1;
        int number = cardNumber[i] - 48;
        if (!(number <= 9 && number >= 0))
        {
            errorMsg = "The provided card number is incorrect! Incorrect symbol has been found!";
            return false;
        }
        int multipicationResult = multiplier * number;
        int obtainedNumber = 0;

        if (multipicationResult > 10)
        {
            while (multipicationResult != 0)
            {
                obtainedNumber += multipicationResult % 10;
                multipicationResult /= 10;
            }
        }
        else if (multipicationResult == 10)
            obtainedNumber = 1;
        else
            obtainedNumber = multipicationResult;

        controlSum += obtainedNumber;
    }
    if (controlSum % 10 != 0)
    {
        errorMsg = "The card number is incorrect, the control sum does not match!";
        return false;
    }
    return true;
}

void CardManager::get_command()
{
    userInterfaceCommandEnum command;
    do {
        userInterfacePtr->get_command(command);
        switch (command) {
            case UI_COMMAND_EXIT:
                break;
            case UI_COMMAND_VIEW:
                this->userInterfacePtr->print_card_deque(cardDeque);
                break;
            case UI_COMMAND_ADD:
                this->add_new_card();
                break;
            case UI_COMMAND_REMOVE_SINGLE:
                this->remove_card_by_id();
                break;
            case UI_COMMAND_REMOVE_EXPIRED:
                this->remove_all_expired_cards();
                break;
            case UI_COMMNAD_REMOVE_ALL:
                this->remove_all_cards();
                break;
            default:
                break;
        };
    } while (command != UI_COMMAND_EXIT);
}

bool CardManager::add_new_card()
{
    bool isValidationOk = true;
    std::string cardNumber;
    unsigned int month;
    unsigned int year;
    std::string firstName;
    std::string lastName;
    std::string errorMsg = "";
    unsigned int currentMonth;
    unsigned int currentYear;

    //get and validate card number
    do {
        if (!isValidationOk)
            userInterfacePtr->print_message(errorMsg);
        userInterfacePtr->get_from_the_user_string(cardNumber,
                                                   "Please enter your card number (16 digits) or put 0 to cancel.");
        if (cardNumber == "0")
            return false;
        isValidationOk = is_card_number_valid(errorMsg, cardNumber);
    } while (!isValidationOk);

    //get and validate month and year
    get_current_month_and_year(currentMonth, currentYear);
    do {
        if (!isValidationOk)
            userInterfacePtr->print_message("Please provide correct expiration date!");
        userInterfacePtr->get_from_the_user_u_int(month,
                                                  "Provide month (1-12) of the expiration date or put 0 to cancel.");
        if (month == 0)
            return false;
        userInterfacePtr->get_from_the_user_u_int(year, "Provide year of the expiration date or put 0 to cancel.");
        if (year == 0)
            return false;
        isValidationOk = is_expiry_date_current(currentMonth, currentYear, month, year);
        if (!isValidationOk)
        {
            std::string confirm;
            userInterfacePtr->get_from_the_user_string(confirm, "You are trying to add card with improper "
                                                                "expiration date, are you sure? Put y to continue, or any other key to cancel");
            if (confirm == "y")
            {
                isValidationOk = true;
            }
        }
    } while (!isValidationOk);

    //get and validate firstName and lastName
    do {
        if (!isValidationOk)
            userInterfacePtr->print_message("First and last name cannot be empty.");
        userInterfacePtr->get_from_the_user_string(firstName,
                                                   "Please enter first name of the card owner or put 0 to cancel.");
        if (firstName == "0")
            return false;
        userInterfacePtr->get_from_the_user_string(lastName,
                                                   "Please enter last name of the card owner or put 0 to cancel.");
        if (lastName == "0")
            return false;
        isValidationOk = !(firstName.length() == 0 || lastName.length() == 0);
    } while (!isValidationOk);

    //add card to deque and save updated deque to file
    Card card = Card(cardNumber, month, year, firstName, lastName);
    cardDeque.push_back(card);

    if (fileHandlerPtr->write_to_file(cardDeque))
    {
        userInterfacePtr->print_message("New card added!");
        return true;
    }
    else
    {
        userInterfacePtr->print_message("!!!ERROR IDx01 writing to file failed!");
    }
    return false;
}

bool CardManager::load_card_list_from_file()
{
    std::string errorMsg;
    auto result = fileHandlerPtr->read_from_file_and_decrypt(cardDeque, errorMsg);
    if (result)
        userInterfacePtr->print_message("The card data file was loaded correctly.");
    else
        userInterfacePtr->print_message("There is a problem with the initialisation of the data storage file. "
                                        "You can ignore this message if you are running this application for the first time.");
    userInterfacePtr->print_message(errorMsg);
    return result;
}

bool CardManager::remove_card_by_id()
{
    unsigned int cardId;
    userInterfacePtr->get_from_the_user_u_int(cardId, "Enter the id of the card to be deleted.");
    for (auto it = cardDeque.begin(); it != cardDeque.end(); ++it)
    {
        if (it->get_card_id() == cardId)
        {
            cardDeque.erase(it);
            if (fileHandlerPtr->write_to_file(cardDeque))
            {
                userInterfacePtr->print_message("Card has been removed successfully.");
                return true;
            }
            else
            {
                userInterfacePtr->print_message("!!!ERROR IDx02, writing to file failed!");
                return false;
            }
        }
    }
    userInterfacePtr->print_message("Card with specified id not found.");
    return false;
}

bool CardManager::remove_all_expired_cards()
{
    std::deque<Card> removedCards;
    unsigned int currentMonth;
    unsigned int currentYear;

    if (cardDeque.empty())
    {
        userInterfacePtr->print_message("There is no saved cards!");
        return false;
    }

    CardManager::get_current_month_and_year(currentMonth, currentYear);

    for (auto it = cardDeque.begin(); it != cardDeque.end(); ++it)
    {
        unsigned int month = it->get_month();
        unsigned int year = it->get_year();
        if (!is_expiry_date_current(currentMonth, currentYear, month, year))
        {
            removedCards.push_back(*it);
        }
    }

    if (removedCards.empty())
    {
        userInterfacePtr->print_message("There is no cards with expired date.");
        return false;
    }
    else
    {
        for (auto it = removedCards.begin(); it != removedCards.end(); ++it)
        {
            unsigned int id = it->get_card_id();
            for (auto iter = cardDeque.begin(); iter != cardDeque.end(); ++iter)
            {
                if (iter->get_card_id() == id)
                {
                    cardDeque.erase(iter);
                    break;
                }
            }
        }
    }
    if (!fileHandlerPtr->write_to_file(cardDeque))
    {
        userInterfacePtr->print_message("!!!ERROR IDx03 Writing of the file failed!");
        return false;
    }
    userInterfacePtr->print_message("Successful removal of expired cards listed below.");
    userInterfacePtr->print_card_deque(removedCards);
    return true;
}

bool CardManager::remove_all_cards()
{
    std::string confirm = "0";
    std::string msg = ("Are you sure you want to remove all cards? "
                       "The operation will not be able to be undone.To abort enter 0, to continue press any key.");
    userInterfacePtr->get_from_the_user_string(confirm, msg);
    if (confirm != "0")
    {
        cardDeque.clear();
        if (!fileHandlerPtr->write_to_file(cardDeque))
        {
            userInterfacePtr->print_message("!!!ERROR IDx04 Writing of the file failed!");
            return false;
        }
        userInterfacePtr->print_message("All cards have been removed.");
        return true;
    }
    else
    {
        userInterfacePtr->print_message("The operation was cancelled.");
        return false;
    }
}