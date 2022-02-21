/*
 * Made for the Course Project in CS230, due March 21st 2022.
 * This work is not under any license attributions, feel free to use freely.
 */

#ifndef CSPROJECT_TABLE_HH
#define CSPROJECT_TABLE_HH

#include "player.hh"
#include "column.hh"
#include "sorting.hh"

#include <vector> // Required for a dynamic collection

// Header file for table class impl
namespace opsx {
    // Class used to wrap a vector of Player-s and simplify actions.
    class Table {
        std::vector<Player> playerVector;

    public:
        void addPlayer(const std::string& fullName,
                       const std::string& games,
                       const std::string& homeRuns,
                       const std::string& strikeOuts);

        void sortThreshold(std::vector<Player>& result, const uint32_t& threshold);

        std::vector<Player>* players();

        static void sort(std::vector<Player>& result, const Column& column, const Sorting& sorting);
    };
}

#endif //CSPROJECT_TABLE_HH
