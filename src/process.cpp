#include <process.hpp>
#include <otree.hpp>
#include "environment.hpp"
#include "leafSelector.hpp"
#include "constraints.hpp"
#include <chrono>
#include <iostream>
#include <unordered_map>
#include <stdexcept>
#include <random>
#include "Fitness.hpp"
#include "output.hpp"
#include <fstream>
#include <filesystem>

Process::Process()
{
    cout<<"constructing process"<<endl;
}

Process::Process(Schedule _partAssign)
    : partAssign(_partAssign)
{

}

Process::~Process()
{
}



void Process::pauseAndWait(){
    //     char input;
    // std::cout << "Press 'c' to continue..." << std::endl;
    // do {
    //     input = std::cin.get(); // Reads a single character from the input
    // } while (input != 'c' && input != 'C'); // Allow both 'c' and 'C'
}

void Process::GenerateInitialPopulation()
{
    cout<<"Generating population..."<<endl;
    int counter = 0; // Ensure this loop cannot run forever
    while(population.size() < Environment::POP_INIT && counter < Environment::POP_INIT * 50)
    {
        //pauseAndWait();

        LeafSelector ls = LeafSelector();
       // cout<<"sending into otree"<<endl;
        //cout<<partialAssignments.assignments.size()<<endl;
        //cout<<"unassigned in partoal"<<endl;
        //cout<<"size of partassign unassigned"<<partAssign.unassigned.size()<<endl;

        if (partialAssignments.unassigned[50] == nullptr) {}
          //  cout<<"oh no! null pointer"<<endl;}


        //partialAssignments.unassigned[50]->printSelf();

        //pauseAndWait();

        Otree tree = Otree(partialAssignments, ls);
        //cout<<"running the tree"<<endl;
        tree.StartTimer();
        Schedule initial = tree.runOtree(tree.root);
        tree.StopTimer();
        //cout<<"found one!!!"<<endl;
        //pauseAndWait();

 

        if (initial.assignments.empty() && !tree.terminate){ // If it ends without preemption, no valid schedule is possible
            cout<<"ERROR: No valid schedule was found."<<endl;
            exit(EXIT_FAILURE);

            break;
            //NOTE FOR THIS: IF ITS A WEIRD BUG, WE CAN JUST MAKE IT CONTINUE INSTEAD OF BREAK, AND JUST GO ON TO THE NEXT SCHEDULE
        }

       // cout<<"initial: "<<endl;
       // cout<<"paired: "<<endl;

        for (pair<GamePractice*, Slot*> assign : initial.assignments){
          //  cout<<"GAME: "<<assign.first->toString()<<" PAIRED WITH SLOT: "<<assign.second->toString()<<endl; 
        }

        //pauseAndWait();


        //initial.eval = Fitness::softConstr(initial);
        //pauseAndWait();
        //cout<<"schedule found"<<endl;
        //initial.eval = Fitness::softConstr(initial);
        initial.eval = initial.Evaluate();
       // cout<<"initial eval: "<<initial.eval<<endl;
        // cout<<"chromotome "<<counter<<endl;

        AddSchedule(initial);
    
       //cout<<"size: "<<initial.assignments.size()<<endl;
       // pauseAndWait();
        counter++;
    }
        
   // cout << "Initial Population Size: " << population.size() << endl;

    for (Schedule &sc : population){
        //  cout<<"sorted population. eval : "<<sc.eval<<endl;
    }
    //cout << "Best before running: " << population[0].eval << endl;
    cout << "Initialization Complete" << endl;
    pauseAndWait();    

   // pauseAndWait();

}

int getRand(int max) {
    static std::random_device rd;  // Non-deterministic random seed
    static std::mt19937 gen(rd()); // Mersenne Twister engine seeded with random_device
    std::uniform_int_distribution<int> dist(0, max - 1); // Define range [0, max-1]

    return dist(gen); // Generate random number
}

void Process::AddSchedule(Schedule& s) // Pass by reference
{
    try
    {
        auto it = lower_bound(population.begin(), population.end(), s); // Find the position where the value should be inserted
        population.insert(it, s);                                       // Insert the value at the determined position
    }
    catch (const std::exception &e)
    {
       // std::cerr << "Error in AddSchedule: " << e.what() << std::endl;
        throw;
    }
}


void Process::SelectExtension()
{
    if (population.size() >= Environment::POP_CAP)
    {
        //cout<<"population size is equal to: "<<Environment::POP_CAP<<"therefore we are deleting"<<endl;
        //pauseAndWait();
        Genocide();
    }
    else
    {
        int r = getRandom(100);

        //float r = 1; //FOR TESTING
        //cout<<"random number is: "<<r<<endl;
        //pauseAndWait();

        if (r < Environment::MUT_PROB*100){
            //cout<<"doing MUTATION as random number is less than"<<Environment::MUT_PROB<<endl;
            //pauseAndWait();
            Irradiate();}
        else{
           // cout<<"pop size: "<<population.size();
           // cout<<"doing CROSSOVER as random number is more than"<<Environment::MUT_PROB<<endl;
            //pauseAndWait();
            MakeSweetLove();
    }
}
}

int Process::getRandom(int max) {
    if (max==0){
      //  cout<<"Max is 0, invalid"<<endl;
        return 0;
    }

        srand(time(0));

        return 1+ (rand() % max);
}




void Process::MakeSweetLove()
{
    try
    {
       // cout<<"Crossover"<<endl;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, ceil(population.size()*Environment::CROSS_PROP));

        int randomIndex = dist(gen);
        int randomIndex2 = dist(gen);

        // Ensure the indices are not the same
        while (randomIndex2 == randomIndex) {
            randomIndex2 = dist(gen);
        }
        //int randomIndex = getRandom(population.size()-1);
        Schedule parentA = population.at(randomIndex);
        //cout<<"got parent a. size: "<<parentA.getSize()<<endl;
        //std::cout<<"first 3 parent A"<<endl;
        
        
        //int randomIndex2 = getRandom(population.size()-1);
        //cout<<"random 1: "<<randomIndex<<"random 2: "<<randomIndex2<<endl;
        //pauseAndWait();
        Schedule parentB = population.at(randomIndex2);
        //cout<<"got parent b. size: "<<parentB.getSize()<<endl;

        // cout<<"pase"<<endl;
        // pauseAndWait();

        LeafSelector checkType = LeafSelector(parentA, parentB);
        // cout<<"leaf selector type: "<<checkType.selectorType<<endl;
        Otree tree = Otree(partialAssignments, LeafSelector(parentA, parentB)); // am I passing these properly????????????????
        tree.StartTimer();
        Schedule child = tree.runOtree(tree.root);
        tree.StopTimer();

            if (child.assignments.empty()){
         //   cout<<"warning: child found no valid schedule was found"<<endl;
            return;
            //NOTE FOR THIS: IF ITS A WEIRD BUG, WE CAN JUST MAKE IT CONTINUE INSTEAD OF BREAK, AND JUST GO ON TO THE NEXT SCHEDULE
        }


        // child.eval = Fitness::softConstr(child);

        // cout<<"child first three: "<<endl;
        int ik=0;
        for (pair<GamePractice*, Slot*> assign : child.assignments){
        
            //  std::cout<<"game: "<<assign.first->toString()<<"slot: "<<assign.second->timeEnd<<endl;
            ik++;
        }

        //        cout<<" SIZE A: "<<parentA.getSize()<<endl;
        //cout<<"size c: "<<child.getSize()<<endl;
        //cout<<"size b "<<parentB.getSize()<<endl;
        //pauseAndWait();
        //cout<<"PARENT A: "<<endl;
        //cout<<"size: "<<parentA.getSize()<<endl;
        for (pair<GamePractice*, Slot*> assign : parentA.assignments){
            //  cout<<assign.first->toString()<<endl;
            // cout<<assign.second->timeStart<<endl;
        }

        // cout<<"child: "<<endl;
        //cout<<"size: "<<child.getSize()<<endl;
        for (pair<GamePractice*, Slot*> assign : child.assignments){
            //  cout<<assign.first->toString()<<endl;
            //cout<<assign.second->timeStart<<endl;
        }

        // cout<<"PARENT B: "<<endl;
        //cout<<"size: "<<parentB.getSize()<<endl;

        for (pair<GamePractice*, Slot*> assign : parentB.assignments){
    //cout<<assign.first->toString()<<endl;
    //           cout<<assign.second->timeStart<<endl;
        }

        // pauseAndWait();
        // assignFitness(child);
        //Fitness fitness = Fitness(&child);
        child.eval = child.Evaluate();
        AddSchedule(child);

        // if (child.eval==population.at(0).eval){
        //       cout<<"crossover resulted in new best eval";
        //     }
    }
    catch (const std::exception &e)
    {
       // std::cerr << "Error in MakeSweetLove: " << e.what() << std::endl;
        throw;
    }
}

void Process::Irradiate()
{
    try
    {
      //  cout<<"doing mutation"<<endl;
        int randomIndex = getRandom(population.size()-1);                     // get random index from entire population
       // cout<<"random index is: "<<randomIndex<<endl;

        Schedule &memberToMut = population.at(randomIndex); // get a schedule from random index
       // cout<<"member to mutate: \n"<<endl;
        
      

        Schedule stripped = memberToMut;                        // this is the stripped schedule to mutate
       // cout<<"initial stripped schedule size: "<<stripped.getSize()<<endl;


       // cout<<"games practices size: "<<gamePractices.size()<<endl;
        for(int i=0; i<std::ceil(memberToMut.getSize() * Environment::MUT_AMOUNT); i++)
        {
            int assToRemoveIndex = getRandom(stripped.assignments.size()-1); // index of the assignment we want to remove from schedule

            GamePractice &gp = gamePractices.at(assToRemoveIndex); // get the GP from the assignment, so that we can use it to remove in schedule.remove(GP)
            if (gp.special)
            {
                continue;
            }
            stripped.RemoveAssignment(gp); // remove the GP from schedul;
        }
    
        Otree tree = Otree(stripped, LeafSelector(memberToMut));
        tree.StartTimer();
        Schedule mutatedSchedule = tree.runOtree(tree.root); // get the mutated schedule for otree
        tree.StopTimer();
        
            if (mutatedSchedule.assignments.empty()){
         //   cout<<"warning: mutated found no valid schedule was found"<<endl;
         //   cout<<"member to mut: "<<memberToMut.getSize()<<endl;
         //   cout<<"stripped: "<<stripped.getSize()<<endl;
         //   pauseAndWait();
            return;
            //NOTE FOR THIS: IF ITS A WEIRD BUG, WE CAN JUST MAKE IT CONTINUE INSTEAD OF BREAK, AND JUST GO ON TO THE NEXT SCHEDULE
        }


       // cout<<"new schedule generated. Size is "<<mutatedSchedule.getSize()<<endl;
        // assignFitness(mutatedSchedule); MAKE SURE TO ADD WHEN CONSTR IS DONE
       // cout<<"former population size was: "<<population.size();
        //mutatedSchedule.eval == Fitness::softConstr(mutatedSchedule);
        mutatedSchedule.eval = mutatedSchedule.Evaluate();
        //mutatedSchedule.eval = Fitness::softConstr(mutatedSchedule);
        AddSchedule(mutatedSchedule);
        // if (mutatedSchedule.eval==population.at(0).eval){
        //     cout<<"mutation resulted in new best eval"<<endl;
        // }

       // cout<<"added schedule to population. New population size is: "<<population.size()<<endl;
    }
    catch (const std::exception &e)
    {
        //std::cerr << "Error in Irradiate: " << e.what() << std::endl;
        throw;
    }
}

void Process::Genocide() // Delete the worst DEL_PROP of the population
{
    try
    {
        size_t delCount = (size_t)(Environment::DEL_PROP * population.size());
        population.erase(population.begin() + delCount, population.end());
    }
    catch (const std::exception &e)
    {
       // std::cerr << "Error in Genocide: " << e.what() << std::endl;
        throw;
    }
}

void Process::Run()
{
   // cout<<"in process.Run()"<<endl;
    GenerateInitialPopulation();
   // cout<<"generated initial population"<<endl;
    int counter = 0;

    auto now = std::chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch());
    long startTime = static_cast<long>(duration.count());
    long currentTime = startTime;

    while (currentTime - startTime < Environment::MAX_RUNTIME)
    {
        if(counter % 10 == 0)
            cout << "Generation " << counter << endl;
        SelectExtension();             // Pick and run an extension rule
        bestResult = population.at(0); // Store the best result at the end of each generation, in case of interruption

        auto now = std::chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch());
        currentTime = static_cast<long>(duration.count());

        //if(bestResult.eval == 0)
        //{
        //    cout << "Best result found, exiting" << endl;
        //    break;
        //}

        // Data gathering to ensure algorithm works as expected
        size_t ninetiethPercentile = (size_t)(population.size() / (float)10);
        size_t median = (size_t)(population.size() / (float)2);
        size_t worst = population.size() - 1;
        bestEvals.push_back(population[0].eval);
        ninetiethPercentiles.push_back(population[ninetiethPercentile].eval);
        medians.push_back(population[median].eval);
        worsts.push_back(population[worst].eval);

        counter++;
    }
    
    cout << "Ran for " << (currentTime - startTime) / 1000 << " seconds" << endl;
    cout << "Completed " << counter << " generations" << endl;
    cout << "Best Schedule: " << endl;
    Output::printSchedule(bestResult);
    cout << "Eval: " << bestResult.eval << endl;
}

void Process::SaveResult(string inputFilename)
{
    std::filesystem::create_directories("./results");

    if (inputFilename.size() >= 4 && inputFilename.substr(inputFilename.size() - 4) == ".txt") 
    {
        // Remove the last 4 characters
        inputFilename.erase(inputFilename.size() - 4);
    }
    string filename = "./results/" + inputFilename + "_output.csv";
    ofstream csvFile(filename);

    if (!csvFile)
    {
        cerr << "Error: Failed to open file " << filename << endl;
        return;
    }

    csvFile << "Best,90th,Median,Worst" << "\n";

    // Output arrays should be the same size
    for(int i = 0; i < bestEvals.size(); i++)
    {
        int best = bestEvals[i];
        int ninetieth = ninetiethPercentiles[i];
        int median = medians[i];
        int worst = worsts[i];
        csvFile << best << "," << ninetieth << "," << median << "," << worst << "\n";
    }

    csvFile.close();
    cout << "Data successfully written to " << filename << "!" << endl;
    return;
}