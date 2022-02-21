/*
 * Made for the Course Project in CS230, due March 21st 2022.
 * This work is not under any license attributions, feel free to use freely.
 */

#include "menu_options.hh"
#include "io_utils.hh"

// Option 1: Output simple data
void opsx::output(Table& table){
    printData(std::cout, table.players(), true);
}

// Option 2: Output by threshold
void opsx::displayWithThresholdSorted(opsx::Table& table) {
    std::vector<opsx::Player> filteredPlayers;
    uint32_t threshold;

    std::cout << "Please enter your home run threshold: ";
    opsx::in<uint32_t>(threshold);

    table.sortThreshold(filteredPlayers, threshold);
    opsx::printData(std::cout, &filteredPlayers, true);
}

// Option 3: Output with calculated info
void opsx::outputWithExtendedStats(Table& table) {
    printData(std::cout, table.players(), false);
}

// Option 4 & 5: Output sorted, save
void opsx::outputSortedByColumn(Table& table, const bool& saveAsFile) {
    std::pair<opsx::Column, opsx::Sorting>* pair = opsx::getColumnSortingType();
    std::vector<opsx::Player> filteredPlayers(*table.players());
    Table::sort(filteredPlayers, pair->first, pair->second);

    if (!saveAsFile) {
        printData(std::cout, &filteredPlayers, false);
        return;
    }
    opsx::saveFile(filteredPlayers);
}

// Delegation function: Gets called periodically by the while loop
// To find out what command the user wants to run
int opsx::delegateMenuType(int& input, Table& table) {
    clearScreen();
    switch (input) {
        case 1: output(table); break;
        case 2: displayWithThresholdSorted(table); break;
        case 3: outputWithExtendedStats(table); break;
        case 4: outputSortedByColumn(table, false); break;
        case 5: outputSortedByColumn(table, true); break;
        case 6: return 1;
        default: return -1;
    }
    askForKeyPress();
    clearScreen();
    return 0;
}
