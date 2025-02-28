#pragma once
#include <stdio.h>
#include <vector>
#include "types.hpp"
#include <format>

namespace Output
{
    void printAssignment(const pair<GamePractice*, Slot*> &assignment)
    {
        // printf("%s\t-->\t%s\n", assignment.first->toString().c_str(), assignment.second->toString().c_str());
        GamePractice *gp = assignment.first;
        Slot *s = assignment.second;
        string day = "";
        switch(s->day){
            case 0:
                day = "MO, ";
                break;
            case 1:
                day = "TU, ";
                break;
            case 2:
                day = "WE, ";
                break;
            case 3: 
                day = "TH, ";
                break;
            case 4: 
                day = "FR, ";
                break;
            default:
                break;
        }
        string gpString = gp->identifier;
        cout << setw(30) << std::left << gpString << 
            setw(30) << std::left << ": " + day + to_string((int)(s->timeStart / 100)) + ":" + to_string((int)(s->timeStart / 10) % 10) + "0" << endl;
    }

    void printSchedule(Schedule& schedule)
    {
        cout << "\n";
        cout << "\033[1;34mEval-value: " << schedule.eval << "\033[0m" << endl;
        for(const pair<GamePractice*, Slot*> &assignment : schedule.assignments)
        {
            printAssignment(assignment);
        }
        cout << endl;
    }
}; // namespace output