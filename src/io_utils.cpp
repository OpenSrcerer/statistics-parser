/*
 * Made for the Course Project in CS230, due March 21st 2022.
 * This work is not under any license attributions, feel free to use freely.
 */

#include "io_utils.hh"

#include <iomanip> // Required for std::setw(), std::left, similar formatting
#include <fstream> // Required for file streams
#include <cmath> // Required for HUGE_VAL

void opsx::clearScreen() {
    #ifdef _WIN32
            std::system("cls");
    #else // Assume POSIX
            std::system("clear");
    #endif
}

// Was Dr. Baglavas' cap actually a Red Sox cap?
// Displays the menu options for the user
void opsx::displayMenuOptions() {
    std::cout << R"(
                               ___
                          .-"``   `'.
                        .'           \
                       /      Dr.B    \
                      |            _.-"-._
                      |     __..-"`       `.
                      '---'``-._           /
                  das           ``--....-'`

    )" << "\n";
    std::cout << "==================================================================\n"
    << "USER MENU: MAJOR LEAGUE BASEBALL STATISTICS FOR THE BOSTON RED SOX\n"
    << "==================================================================\n"
    << "1. Display simple statistics for the data inserted\n"
    << "2. Display simple statistics for players with home runs above a certain threshold, in alphabetical order\n"
    << "3. Calculate HR/G & SO/G, display\n"
    << "4. Sort by a field of your choosing\n"
    << "5. Save the data into a new file\n"
    << "6. Exit the program"
    << std::endl;
}

void opsx::displayColumnSortingOptions() {
    std::cout << "==================================================================\n"
    << "Select the column to sort your data by:\n"
    << "==================================================================\n"
    << "1. First name\n"
    << "2. Last name\n"
    << "3. Total games\n"
    << "4. Home runs\n"
    << "5. Strikeouts\n"
    << "6. Home runs per game\n"
    << "7. Strikeouts per game\n"
    << std::endl;
}

void opsx::displayOrderSortingOptions() {
    std::cout << "==================================================================\n"
    << "Should your data be sorted in:\n"
    << "==================================================================\n"
    << "1. Ascending order\n"
    << "2. Descending order\n"
    << std::endl;
}

void opsx::askForKeyPress() {
    std::cout << "\nPress Enter to go back to the main menu..." << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n'); // Ignore line
}

void opsx::askForKeyPress(const char msg[]) {
    std::cout << msg << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n'); // Ignore line
}

// Format player stats nicely for a table row (prints just a row)
void opsx::prettyPrintStats(std::ostream& stream, const std::string arr[], const bool& simple) {
    for (size_t i = 0; i < ((simple) ? 5 : 7); ++i) {
        stream << std::setw(12) << std::left << arr[i];
        if (i != 6) {
            stream << "| ";
        }
    }
    stream << "\n";
}

// Prints a full vector of players
void opsx::printData(std::ostream& stream, std::vector<opsx::Player>* players, const bool& simple) {
    std::string* arr = opsx::Player::serializeDefault(); // Retrieve default player
    opsx::prettyPrintStats(stream, arr, simple);
    delete[] arr; // Delete heap memory ptr

    stream << ((simple) ? "----------------------------------------------------------------------\n" :
    "-----------------------------------------------------------------------------------------------\n");
    if (players->empty()) {
        stream << std::setw(40) << std::right << "No Data!\n";
    }
    for (auto& player : *players) {
        arr = player.serialize(simple);
        opsx::prettyPrintStats(stream, arr, simple);
        delete[] arr; // Delete heap memory ptr
    }
}

// Saves a file to the disk
void opsx::saveFile(std::vector<Player> &players) {
    std::string fileName;

    std::cout << "Please insert the name of the file to be saved (default: \"stefani_red_sox_stats.txt\"): ";
    std::getline(std::cin, fileName);
    if (fileName.empty()) {
        fileName = std::string("stefani_red_sox_stats.txt");
    }

    std::ofstream stream(fileName);
    printData(stream, &players, false); // Buffer stream
    stream.flush(); // Write contents to disk
    stream.close(); // Exit stream gracefully
    std::cout << "File \"" << fileName << "\" successfully saved!\n";
}

// Imports data from a file to the Table object
void opsx::populateTable(std::ifstream* statsFile, opsx::Table& table) {
    std::vector<std::string> tokens;
    std::string line;

    while (std::getline(*statsFile, line)) {
        std::stringstream ss(line);

        while (std::getline(ss, line, '\t')) {
            tokens.push_back(line);
        }

        table.addPlayer(
                tokens.at(0),
                tokens.at(1),
                tokens.at(2),
                tokens.at(3)
        );
        tokens.clear();
    }
    statsFile->close();
    delete statsFile; // Deallocate heap memory
}

void opsx::appendColumnError(std::string* errorMessage, int& lineCounter, std::string& line, std::string& colName) {
    errorMessage->append("Error: Column\" ")
            .append(colName)
            .append("\" should have number value")
            .append("\nat line (")
            .append(std::to_string(lineCounter))
            .append(") -> \"")
            .append(line)
            .append("\"");
}

// Check if a given string is a number (decimal numbers allowed)
bool opsx::isNumber(const std::string& string) {
    char* end = nullptr;
    double val = strtod(string.c_str(), &end);
    return end != string.c_str() && *end == '\0' && val != HUGE_VAL;
}

// Top-level function to check if a file is properly formatted
bool opsx::verifyFileIntegrity(std::string& filePath) {
    std::string* errorMessage = opsx::parseFile(filePath);

    if (!errorMessage->empty()) {
        std::cout << *errorMessage;
        return false;
    } else {
        return true;
    }
}

// Check if a given string is in the format text_text
bool opsx::isFirstLast(const std::string &string) {
    uint8_t iterations = 0;
    std::string token;
    std::stringstream ss(string);

    while (std::getline(ss, token, '_')) {
        ++iterations;
    }

    return iterations == 2;
}

// Output information to the user regarding file verification
bool opsx::interactWithUserFileIntegrity(std::string& filePath, std::ifstream* stream) {
    std::cout << "File found! Verifying statistics file structure & data integrity...\n\n";
    bool integrity = verifyFileIntegrity(filePath);
    if (integrity) {
        std::cout << "Statistics file data verified!\n\n";
    } else {
        stream->close();
        askForKeyPress("\n\nPress Enter to continue...");
        clearScreen();
    }
    return integrity;
}

// Delegation function for columns
opsx::Column opsx::delegateColumnType(int &input) {
    switch (input) {
        case 1: return Column::first;
        case 2: return Column::last;
        case 3: return Column::games;
        case 4: return Column::hruns;
        case 5: return Column::souts;
        case 6: return Column::hrpg;
        case 7: return Column::sopg;
        default: return Column::unknown;
    }
}

// Delegation function for sorting types
opsx::Sorting opsx::delegateSortingType(int &input) {
    switch (input) {
        case 1: return Sorting::ascending;
        case 2: return Sorting::descending;
        default: return Sorting::invalid;
    }
}

// Function that checks if the input file is properly formatted
std::string* opsx::parseFile(std::string& filePath) {
    auto* errorMessage = new std::string(""); // Error message to return, empty if no errors
    std::ifstream fileStream(filePath);
    std::vector<std::string> tokens;
    std::string line;
    std::string colType;
    int lineCounter = 1;

    while (std::getline(fileStream, line)) { // Read every line
        std::string lineCopy = std::string(line);
        lineCopy.erase(std::remove(lineCopy.begin(), lineCopy.end(), '\r'), lineCopy.end());
        lineCopy.erase(std::remove(lineCopy.begin(), lineCopy.end(), '\n'), lineCopy.end());
        std::stringstream ss(line);

        while (std::getline(ss, line, '\t')) { // Read every token
            // Sanitize string
            line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
            line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
            tokens.push_back(line); // Push sanitized string
        }

        // Verify number of tokens in line
        if (tokens.size() < 4) { // If row has less than 4 columns
            errorMessage->append("Error: Invalid number of tokens. Required 4 (name->str, games->num, homeruns->num, strikeouts->num), given ")
                    .append(std::to_string(tokens.size()))
                    .append("\nat line (")
                    .append(std::to_string(lineCounter))
                    .append(") -> \"")
                    .append(lineCopy)
                    .append("\"")
                    .append("\nHint: Make sure your file is tab-delimited.");
            break;
        } else if (!isFirstLast(tokens.at(0))) { // If first_last column is improperly formatted
            errorMessage->append("Error: Column \"")
                    .append("firstname_lastname")
                    .append("\" is improperly formatted")
                    .append("\nat line (")
                    .append(std::to_string(lineCounter))
                    .append(") -> \"")
                    .append(lineCopy)
                    .append("\"");
            break;
        } else if (!isNumber(tokens.at(1))) { // Improper format for numbers
            colType = "games";
            appendColumnError(errorMessage, lineCounter, lineCopy, colType);
            break;
        } else if (!isNumber(tokens.at(2))) { // Improper format for numbers
            colType = "home runs";
            appendColumnError(errorMessage, lineCounter, lineCopy, colType);
            break;
        } else if (!isNumber(tokens.at(3))) { // Improper format for numbers
            colType = "strikeouts";
            appendColumnError(errorMessage, lineCounter, lineCopy, colType);
            break;
        }
        tokens.clear();
        ++lineCounter;
    }
    return errorMessage;
}

std::ifstream* opsx::scanForFileElseAsk() {
    int input;
    bool integrity;
    std::string filePath("stefani_red_sox.txt");
    auto* stream = new std::ifstream(filePath);

    if (!stream->good()) { // Try reading a file for the first time
        // If not ok, show user
        std::cout << "Oops! I automagically scanned the current directory "
        << "but could not find the default file (\"stefani_red_sox.txt\").\n\n";
    } else { // Otherwise verify integrity
        std::cout << "Successfully loaded default file \"stefani_red_sox.txt\".\n";
        integrity = interactWithUserFileIntegrity(filePath, stream);
    }

    while (!stream->good() || !integrity) { // If file loading failed or file is corrupt
        std::cout << "The program needs to import a valid file to proceed.\nWhat would you like to do next?\n\n"
                  << "1. Re-import the selected file (Selected file: \""
                  << filePath << "\")\n"
                  << "2. Insert the absolute path manually (Example: \"/path/to/file/file.txt\", no quotes)\n"
                  << "3. Exit this program\n\n"
                  << "Enter your option (1, 2, 3): ";
        in<int>(input);

        switch (input) {
            case 1: // Re-import
                stream->clear();
                stream->open("stefani_red_sox.txt");
                break;
            case 2: { // Absolute Path
                std::cout << "Please insert the file path (Example: \"/path/to/file/file.txt\", no quotes):\n";
                in<std::string>(filePath);
                filePath.erase(std::remove_if(filePath.begin(), filePath.end(), isspace), filePath.end());
                stream->clear();
                stream->open(filePath);
                break;
            }
            // Exit
            case 3: std::cout << "Goodbye!" << std::endl; exit(-1);
            default: // Invalid Input
                std::cout << "\nI did not understand your input.\nPlease make sure to enter just \"1\", \"2\", or \"3\".";
                askForKeyPress("\n\nPress Enter to continue...");
                clearScreen();
        }

        clearScreen();
        if (!stream->good()) {
            std::cout << "Could not find the file \"" << filePath << "\".\n";
        } else {
            integrity = interactWithUserFileIntegrity(filePath, stream);
        }
    }
    return stream;
}

// Return a Pair object, where the first object contains the selected column
// And the second object the sorting type
std::pair<opsx::Column, opsx::Sorting>* opsx::getColumnSortingType() {
    int input;
    Column column = Column::unknown;
    Sorting sorting = Sorting::invalid;

    opsx::displayColumnSortingOptions();
    std::cout << "Enter your input: ";
    in<int>(input);

    while (column == Column::unknown) {
        column = opsx::delegateColumnType(input);
        clearScreen();
        if (column == Column::unknown) {
            std::cout << "I did not understand your input.\nPlease make sure to enter just \"1\" - \"7\".\n\n";
            opsx::displayColumnSortingOptions();
            std::cout << "Enter your input: ";
            in<int>(input);
        }
    }

    opsx::displayOrderSortingOptions();
    std::cout << "Enter your input: ";
    in<int>(input);

    while (sorting == Sorting::invalid) {
        sorting = opsx::delegateSortingType(input);
        clearScreen();
        if (sorting == Sorting::invalid) {
            std::cout << "I did not understand your input.\nPlease make sure to enter just \"1\" or \"2\".\n\n";
            opsx::displayOrderSortingOptions();
            std::cout << "Enter your input: ";
            in<int>(input);
        }
    }

    return new std::pair(column, sorting);
}