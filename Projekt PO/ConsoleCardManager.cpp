/**
 * Author: Mateusz Pazdro
 */

#include "card_manager.h"
#include "handlers.h"

const std::string cardDataFilePath = "cards.json";

int main()
{
    EncryptionHandler* ehPtr = new EncryptionHandler();
    FileHandler *fhPtr = new FileHandler(cardDataFilePath, ehPtr);
    CardManagerUserInterface* uiPtr = new CardManagerUICommandLine();
    CardManager cardManager(fhPtr, uiPtr);

    cardManager.load_card_list_from_file();
    cardManager.get_command();

    delete uiPtr;
    delete fhPtr;
    delete ehPtr;

    return 0;
}