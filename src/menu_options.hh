/*
 * Made for the Course Project in CS230, due March 21st 2022.
 * This work is not under any license attributions, feel free to use freely.
 */

#ifndef CSPROJECT_MENU_OPTIONS_HH
#define CSPROJECT_MENU_OPTIONS_HH

#include "table.hh"
#include "sorting.hh"

// Header file for menu options.
namespace opsx {
    void output(Table& table);

    void displayWithThresholdSorted(Table& table);

    void outputWithExtendedStats(Table& table);

    void outputSortedByColumn(Table& table, const bool& saveAsFile);

    int delegateMenuType(int& input, Table& table);
}

#endif //CSPROJECT_MENU_OPTIONS_HH
