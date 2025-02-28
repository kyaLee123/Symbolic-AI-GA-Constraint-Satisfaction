

#pragma once
#include <vector>
#include "types.hpp"
#include "environment.hpp"
#include <unordered_map>


class Fitness
{

    public:
    Schedule* schedule;
    Fitness(Schedule* schedule);
    int softConstr();
    int softConstrNewAssignment(Schedule schedule, pair<GamePractice*, Slot*>);
    int getMinFilledPenalty(Slot* slot);
    int getPrefPenalty(Slot* slot);
    int getPairPenalty(Slot* slot);
    int getSecPenalty(Slot* slot);
    void pauseAndWait();
    vector<GamePractice*> checked;



};