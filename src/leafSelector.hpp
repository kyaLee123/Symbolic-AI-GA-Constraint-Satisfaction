#pragma once

#include <types.hpp>
#include <vector>
#include <unordered_map>
#include <stack>

using namespace std;


class LeafSelector
{
private:
   // Schedule initialSchedule;
   // Schedule secondSchedule;
   // Slot getCrossoverSlot(vector<Slot> openSlotOptions);
   // Slot getMutationSlot(vector<Slot> openSlotOptions);
   // Slot getInitSlot(vector<Slot> openSlotOptions);
    Schedule parentA;
    Schedule parentB;
    Schedule originalMutSchedule;
    stack<Slot*> getMutationStack(GamePractice* emptyElement, vector<Slot*> &openSlotOptions, bool evening);
    stack<Slot*> getCrossoverStack(GamePractice* emptyElement, vector<Slot*> &openSlotOptions, bool evening);


    //vector<Slot> filterHardConstraints(GamePractice emptyElement, vector<Slot> &slotsToFilter);
//this is just for testing purposes
public:
    string selectorType; // TODO: Change to enum
    LeafSelector(Schedule &A, Schedule &B); //crossover
    LeafSelector(Schedule &O); //mutation
    LeafSelector(); //initialize

    //FOR CROSSOVER: YOU NEED SCHEDULES PARENTA, PARENTB TO DO FCROSSOVEREVAL
    //FOR MUTATION: YOU JUST NEED THE ORIGINAL SCHEDULE TO COMPARE VALUES TO, CAN OPTIMIZE A LOT FOR MUTATION FYI
    //FOR INITIALIZE: YOU DONT NEED ANY REFERENCE SCHEDULE. MAYBE OTREE CAN HANDLE THIS BY JUST MAKING THE STACK EQUAL TO OPENSLOTLIST,
    //OR LEAF CAN HANDLE BY JUST MAKING THE INPUT OPENSLOTS INTO A QUEUE IDC. WHATEVER U THINK.

    //LeafSelector(string inputType, Schedule& inputSchedule, Schedule& referenceSchedule);
    ~LeafSelector();
    stack<Slot*> getStack(GamePractice* emptyElement, vector<Slot*> &openSlotOptions, bool isEvening);
    //note: this will return a stack with the "best" slots on top, and the "worst" on bottom,
};
