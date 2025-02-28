#pragma once
#include <vector>
#include <map>
#include "types.hpp"
#include <fstream>

using namespace std;

const bool DEBUG = true;

class GamePractice;
class Slot;

// Global data
extern vector<std::string> leagues;
extern vector<GamePractice> gamePractices;
extern vector<GamePractice *> games;
extern vector<GamePractice *> practices;

extern vector<Slot> slots;
extern vector<Slot *> gameslots;
extern vector<Slot *> practiceslots;

class Schedule;
extern Schedule partialAssignments;
// extern vector<Slot> slots;

// Penalties recieved via input.
struct Penalties
{
    int gamemin;
    int practicemin;
    int notpaired;
    int pen_section;
};
extern Penalties penalties;


struct Weights
{
    int wMinFilled;
    int wPref;
    int wPair;
    int wSecDiff;
};
extern Weights weights;



namespace Environment
{
    extern size_t POP_INIT;         // Initial population
    extern size_t POP_CAP;          // Max population before running delete
    extern float MUT_PROB;          // Chance of selecting mutation instead of crossover
    extern float MUT_AMOUNT;        // Proportion of schedule to be changed by mutation
    extern float CROSS_PROP;        // Proportion of population that can be selected by crossover
    extern float DEL_PROP;          // Proportion of the population that gets deleted

    extern long MAX_RUNTIME;        // Limits the program's run time in seconds
    extern int MAX_OTREE_TIME;      // In seconds

    void LoadEnvironmentConfig();
};