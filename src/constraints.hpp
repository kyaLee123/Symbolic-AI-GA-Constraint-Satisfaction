#pragma once
#include <vector>
#include "types.hpp"
#include "environment.hpp"

class Constraints
{
    public:
        static bool Constr(Schedule* schedule); // Meets all constraints and complete
        static bool PartConstr(Schedule* schedule); // Meets hard constraints
        static int GetEval(Schedule schedule); // Compute soft constraints
        void PAUSE();
        static bool PseudoConstr(Schedule* schedule, GamePractice* gamePractice, Slot* slot);
        // static bool comp(); // compare slots to sort them by time and date
};