/*
 * Made for the Course Project in CS230, due March 21st 2022.
 * This work is not under any license attributions, feel free to use freely.
 */

#ifndef CSPROJECT_PLAYER_HH
#define CSPROJECT_PLAYER_HH

#include "column.hh"

#include <string> // Required for std::string
#include <unordered_map> // Required for std::undordered_map
#include <any> // Required for std::any

// Header file for the player class
namespace opsx {
    class Player {
        std::unordered_map<Column, std::any> columnMap;

    public:
        Player(const std::string& fullName,
               const std::string& games,
               const std::string& homeRuns,
               const std::string& strikeOuts
        );

    private:
        static std::string doubleToString(const double& val);

    public:
        std::any operator[](const Column& column);

        std::string firstName();

        std::string lastName();

        uint32_t games();

        uint32_t homeRuns();

        uint32_t strikeOuts();

        double gameHomeRuns();

        double gameStrikeOuts();

        bool hasMoreHomeRunsThan(const uint32_t& hrs);

        std::string* serialize(const bool& simple);

        static std::string* serializeDefault();

        static bool comparator(Player& first, Player& second, const Column& column);
    };
}

#endif //CSPROJECT_PLAYER_HH
