/*
 * Made for the Course Project in CS230, due March 21st 2022.
 * This work is not under any license attributions, feel free to use freely.
 */

#include "table.hh"

// Method to add a player to the vector in the table
void opsx::Table::addPlayer(const std::string& fullName,
               const std::string& games,
               const std::string& homeRuns,
               const std::string& strikeOuts
) {
    // Vector method that takes the arguments for a Player Constructor
    // and constructs an instance by itself, then adds to vector
    playerVector.emplace_back(fullName, games, homeRuns, strikeOuts);
}

// Return a copy of the players vector with a HR larger than a threshold, sorted by first name,
void opsx::Table::sortThreshold(std::vector<Player>& result, const uint32_t& threshold) {
    std::copy_if(playerVector.begin(),playerVector.end(),
                 std::back_inserter(result),
                 [threshold] (Player& pl) { return pl.hasMoreHomeRunsThan(threshold); });
    sort(result, Column::first, Sorting::ascending);
}

// Return a pointer to this table's player vector
std::vector<opsx::Player>* opsx::Table::players() {
    return &this->playerVector;
}

// Sort this table by a given column and ascending/descending sort type
void opsx::Table::sort(std::vector<Player>& result, const Column& column, const Sorting& sorting) {
    std::sort(result.begin(), result.end(), [column, sorting](Player& p1, Player& p2) {
        bool comparator = Player::comparator(p1, p2, column);
        return (sorting == Sorting::ascending) == comparator;
    });
}
