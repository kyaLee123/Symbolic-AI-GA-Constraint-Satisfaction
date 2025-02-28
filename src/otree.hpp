#pragma once

#include "types.hpp"
#include <vector>
#include <unordered_map>
#include <string>
#include "leafSelector.hpp"
#include <thread>

using namespace std;

class Node {
public:
    Schedule schedule;
    vector<GamePractice*> remainingSpecialGPs;
    vector<GamePractice*> remainingGPs;
    vector<GamePractice*> evenings;
    unordered_map<GamePractice*, vector<Slot>> invalidSlots;
    int deep;

    bool isRoot;
    Node();

    Node(Schedule s, vector<GamePractice*> gps, vector<GamePractice *> sgps, vector<GamePractice*> evening);
    ~Node();
};

class Otree {
    private:
        void getCountsPartAssign(Schedule part);
        Schedule s;
        LeafSelector ls;
        vector<Slot*> openGameSlots;
        vector<Slot*> openPracticeSlots;
        GamePractice* getRandomIndex( std::vector<GamePractice*>& remainingGPs, vector<GamePractice*> &notSpecialGPs, vector<GamePractice*> &evenings);
        void updateSlotCountsAndAvailability(Schedule nodeSchedule, GamePractice* currentGP, Slot *slot, int delta);
        vector<Slot*> openGameSlotsAge;
        vector<Slot*> openPracticeSlotsAge;
        vector<Slot*> eveningSlots;
        int depth;

        bool stopWorker = false;
        std::thread worker;
        void ExecutionLimiter(); // Times execution, breaking from Otree when the max time has elapsed
    public:
    Node root;
        bool terminate = false; // Tells the otree to stop and return a null schedule

        Schedule result;
        Schedule runOtree(Node &n);
        Otree(Schedule inputSchedule, LeafSelector leafSelector);
        ~Otree();

        void StartTimer(); // Starts execution timer
        void StopTimer(); // Stops execution timer
};

