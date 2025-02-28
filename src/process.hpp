#pragma once
#include <vector>
#include "types.hpp"
#include "environment.hpp"
#include "leafSelector.hpp"

class Process
{
    private:
        vector<int> worsts;
        vector<int> medians;
        vector<int> ninetiethPercentiles;
        vector<int> bestEvals;
    public:
        const size_t maxPop = 1000;


        const float deleteProportion = 0.5; // The proportion of the population remaining after running delete
        const Schedule partAssign;
        vector<Schedule> population;
        Schedule bestResult;
        void GenerateInitialPopulation();
        void AddSchedule(Schedule &s); // Add a schedule such that the vector remains sorted by eval
        void SelectExtension(); // Decides which extension rule to run
        void MakeSweetLove(); //AKA crossover
        void Irradiate(); // AKA mutation
        void Genocide(); // AKA deleta: Delete the worst DEL_PROP of the population
        int getRandom(int max);
        void pauseAndWait();
        Process();
        Process(Schedule partAssign);
        ~Process();
        int getRand(int max);
        void Run(); // Run the search model
        Schedule BestResult(); // Return the current best result
        void SaveResult(string inputFilename); // Save the results to a file for graphing
};