/*
 * Made for the Course Project in CS230, due March 21st 2022.
 * This work is not under any license attributions, feel free to use freely.
 */

#include "io_utils.hh"
#include "table.hh"
#include "menu_options.hh"

using namespace opsx;

int main() {
    // Initialize "top-level" variables
    std::ifstream* fileStream;
    Table table;
    int terminalSignal;
    int input;

    fileStream = scanForFileElseAsk(); // Scan for the file
    populateTable(fileStream, table); // Add data to Table

    while (true) { // Run the program until user exists.
        displayMenuOptions();
        std::cout << "\nPlease enter an option: ";
        in<int>(input);

        terminalSignal = delegateMenuType(input, table);

        if (terminalSignal == 1) {
            break;
        }
    }

    std::cout << "Goodbye!\n";
}