/*
 * Made for the Course Project in CS230, due March 21st 2022.
 * This work is not under any license attributions, feel free to use freely.
 */

#include "player.hh"

#include <sstream> // Required for std::stringstream

// Constructor for the Player class
opsx::Player::Player(
        const std::string &fullName,
        const std::string &games,
        const std::string &homeRuns,
        const std::string &strikeOuts)
{
    std::stringstream ss(fullName);
    std::string token;

    std::getline(ss, token, '_'); // Read First Name
    this->columnMap[Column::first] = token;
    std::getline(ss, token, '_'); // Read Last Name
    this->columnMap[Column::last] = token;

    // Parse and read numerical data
    this->columnMap[Column::games] = (uint32_t) std::stoul(games);
    this->columnMap[Column::hruns] = (uint32_t) std::stoul(homeRuns);
    this->columnMap[Column::souts] = (uint32_t) std::stoul(strikeOuts);
    this->columnMap[Column::hrpg] = (double) this->homeRuns() / this->games();
    this->columnMap[Column::sopg] = (double) this->strikeOuts() / this->games();
}

// Converts a double to a string
std::string opsx::Player::doubleToString(const double &val) {
    std::string strVal = std::to_string(val);
    strVal.erase(strVal.length() - 3);
    return strVal;
}

// Operator overload for map access
std::any opsx::Player::operator[](const opsx::Column &column) {
    return this->columnMap[column];
}

std::string opsx::Player::firstName() {
    return std::any_cast<std::string>(columnMap[Column::first]);
}

std::string opsx::Player::lastName() {
    return std::any_cast<std::string>(columnMap[Column::last]);
}

uint32_t opsx::Player::games() {
    return std::any_cast<uint32_t>(columnMap[Column::games]);
}

uint32_t opsx::Player::homeRuns() {
    return std::any_cast<uint32_t>(columnMap[Column::hruns]);
}

uint32_t opsx::Player::strikeOuts() {
    return std::any_cast<uint32_t>(columnMap[Column::souts]);
}

double opsx::Player::gameHomeRuns() {
    return std::any_cast<double>(columnMap[Column::hrpg]);
}

double opsx::Player::gameStrikeOuts() {
    return std::any_cast<double>(columnMap[Column::sopg]);
}

bool opsx::Player::hasMoreOrEqualHomeRunsThan(const uint32_t &hrs) {
    return this->homeRuns() >= hrs;
}

// Returns an array representation of this Player's data
std::string *opsx::Player::serialize(const bool &simple) {
    return new std::string[7] {
            {this->firstName()}, {this->lastName()},{std::to_string(this->games())},
            {std::to_string(this->homeRuns())},{std::to_string(this->strikeOuts())},
            {(simple) ? "" : doubleToString(this->gameHomeRuns())},
            {(simple) ? "" : doubleToString(this->gameStrikeOuts())}
    };
}

// Returns an array representation of a player filled with placeholder data
std::string *opsx::Player::serializeDefault() {
    return new std::string[7] {{"First Name"}, {"Last Name"},
                               {"Games"}, {"Home Runs"}, {"Strikeouts"},
                               {"HR/Game"}, {"Strikeouts/Game"}};
}

// Method that compares two players by one column
bool opsx::Player::comparator(Player& first, Player& second, const Column& column) {
    std::any firstColumn = first[column];
    std::any secondColumn = second[column];

    switch (column) {
        // Case when columns are strings
        case opsx::first: case opsx::last: {
            // Cast to string
            auto firstString = std::any_cast<std::string>(first[column]);
            auto secondString = std::any_cast<std::string>(second[column]);

            // Make both strings lowercase
            std::transform(firstString.begin(), firstString.end(), firstString.begin(), ::tolower);
            std::transform(secondString.begin(), secondString.end(), secondString.begin(), ::tolower);

            return firstString.compare(secondString) < 0;
        }
        // Both columns are doubles
        case opsx::hrpg: case opsx::sopg: {
            // Cast to double
            auto firstDouble = std::any_cast<double>(first[column]);
            auto secondDouble = std::any_cast<double>(second[column]);
            return firstDouble < secondDouble;
        }
        // Both columns are unsigned integers
        default: {
            // Cast to uint
            auto firstUint = std::any_cast<uint32_t>(first[column]);
            auto secondUint = std::any_cast<uint32_t>(second[column]);
            return firstUint < secondUint;
        }
    }
}
