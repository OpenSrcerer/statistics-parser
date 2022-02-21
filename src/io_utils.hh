/*
 * Made for the Course Project in CS230, due March 21st 2022.
 * This work is not under any license attributions, feel free to use freely.
 */

#ifndef CSPROJECT_IO_UTILS_HH
#define CSPROJECT_IO_UTILS_HH

#include "player.hh"
#include "table.hh"

#include <vector> // Required for a dynamic collection
#include <iostream> // Required for std::cin and std::cout
#include <fstream> // Required for file streams
#include <limits> // Required for numeric_limits

// Headers for io_utils.
namespace opsx {
    void clearScreen();

    void displayMenuOptions();

    void displayColumnSortingOptions();

    void displayOrderSortingOptions();

    void askForKeyPress();

    void askForKeyPress(const char msg[]);

    void prettyPrintStats(std::ostream& stream, const std::string arr[], const bool& simple);

    void printData(std::ostream& stream, std::vector<opsx::Player>* players, const bool& simple);

    void saveFile(std::vector<Player>& players);

    void populateTable(std::ifstream* statsFile, opsx::Table& table);

    template <typename T>
    void in(T& value) {
        std::cin >> value;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    void appendColumnError(std::string* errorMessage, int& lineCounter, std::string& line, std::string& colName);

    bool isNumber(const std::string& string);

    bool isFirstLast(const std::string& string);

    bool verifyFileIntegrity(std::string& filePath);

    bool interactWithUserFileIntegrity(std::string& filePath, std::ifstream* stream);

    Column delegateColumnType(int& input);

    Sorting delegateSortingType(int& input);

    std::string* parseFile(std::string& filePath);

    std::ifstream* scanForFileElseAsk();

    std::pair<Column, Sorting>* getColumnSortingType();
}

#endif //CSPROJECT_IO_UTILS_HH
