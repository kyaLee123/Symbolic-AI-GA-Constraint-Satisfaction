#include "otree.hpp"
#include "random"
#include <algorithm>
#include <leafSelector.hpp>
#include <string>
#include <iostream>
#include <ctime>
#include <vector>
#include <string>
#include <otree.hpp>
#include <random> // For random number generation
#include "environment.hpp"
#include <vector>
#include <random>
#include <algorithm>
#include <iostream>
#include <bits/stdc++.h>
#include <thread>
#include <chrono>


//#include "cotSpecial;nstraints.hpp"

using namespace std;

Node::Node(){}

Node::Node(Schedule s, vector<GamePractice *> gps, vector<GamePractice *> sgps, vector<GamePractice*> evening) : schedule(s), remainingGPs(gps), remainingSpecialGPs(sgps), evenings(evening)
{

}
Node::~Node(){}



void pauseAndWait(){
    char input;
    std::cout << "Press 'c' to continue..." << std::endl;
    do {
        input = std::cin.get(); // Reads a single character from the input
    } while (input != 'c' && input != 'C'); // Allow both 'c' and 'C'
}


Otree::~Otree(){}

Otree::Otree(Schedule inputSchedule, LeafSelector leafSelector) : s(inputSchedule), ls(leafSelector), depth(0)
{
    vector<GamePractice*> specialGPs; //special game practices (age 15-19)
    vector<GamePractice*> notSpecialGPs; //general game practices
    vector<GamePractice*> evenings; //evening game practices

        for (Slot &sl : slots){ //iterate through all slots
            if (sl.timeStart>=1800){ //if the start time is evening, we add it to evening slots
                eveningSlots.push_back(&sl); 
            } 
        if (sl.type==GPType::GAME){ // if the type is a game
            if (!(sl.day==1)||!(sl.timeStart==1100)){ //and it is not TUE at 11:00
        openGameSlots.push_back(&sl); //we are safe to add to game slots
        openGameSlotsAge.push_back(&sl);} //these are special age game slots, that include U15-19
        }
        else {
        openPracticeSlots.push_back(&sl); //since not a game, it is a practice, and add to open practice slots
        }
    }
    

    for (GamePractice* gp : s.unassigned){ //get all unassigned GPs from schedule
    
    std::string numStr = std::to_string(std::abs(gp->division));  //numStr is a string of the division

    if ((gp->type==GPType::PRACTICE&&gp->special==1)){
        continue; //if its a special GP, we dont need to add it (in partassign already)
    }
    

     if ((gp->age==15||gp->age == 16||gp->age==17||gp->age==19)&&(gp->underOver==0)){ //if U 15-19
        if (gp->type==GPType::GAME){ //and game
            specialGPs.push_back(gp); //we add it to special GP list, to iterate over first
            continue;}   //continue past for loop
        }
    if (numStr[0] == '9'){ //if the first digit of numStr is 9, we have an evening GP
        evenings.push_back(gp); //we add it to the evenings list
        continue;
    } 
    
        notSpecialGPs.push_back(gp); //otherwise, it is not a special gp, and is added to the regular list
    }
    
    root = Node(s, notSpecialGPs, specialGPs, evenings); //create the root node, to be called by methods in process
}

void Otree::ExecutionLimiter()
{
    float elapsed = 0; //Keep track of elapsed seconds
    while (!stopWorker && elapsed < Environment::MAX_OTREE_TIME) 
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        elapsed += 10.f / 1000.f;
    }

    if (!stopWorker) { //Only set terminate if not stopped early
        terminate = true;
    }
}

void Otree::StartTimer()
{
    worker = std::thread([this]() {
        this->ExecutionLimiter();
    });
}

void Otree::StopTimer()
{
    stopWorker = true;
    if (worker.joinable()) {
        worker.join(); // Wait for the thread to finish
    }
}
        

//Function to get a random gamePractice by index of a vector
GamePractice* Otree::getRandomIndex( std::vector<GamePractice *> &specialGPs, vector< GamePractice*> &notSpecialGPs, vector<GamePractice*> &evenings)
{
    static bool seeded = false;
    if (!seeded)
    {
        std::srand(static_cast<unsigned int>(std::time(0))); // Seed the random number generator with current time
        seeded = true;
    }

    if (!specialGPs.empty()){ //if we have special gps remaining
//        cout<<"returning special one"<<endl;

            int randomIndex = std::rand() % specialGPs.size(); //get a random index based off of size

            GamePractice* toReturn = specialGPs[randomIndex]; //get gp from the index
            specialGPs.erase(specialGPs.begin()+randomIndex); //remove the gp from the list
            
            return toReturn; //return the gp
    }

       if (!evenings.empty()){ //repeat for evenings
            int randomIndex = std::rand() % evenings.size();
            GamePractice* toReturn = evenings[randomIndex];
          //  cout<<"before evening erase size: "<<evenings.size()<<endl;
            evenings.erase(evenings.begin()+randomIndex);
           // cout<<"returning an evening one. Evenings size: "<<evenings.size()<<endl;
            return toReturn;
    }

    if (notSpecialGPs.empty())
    {        
        std::cerr << "Error: remainingGPs is empty!" << std::endl; //if we get to this point, there has been an error
        return NULL;
    }

    else { //repeat for normal gps
            int randomIndex = std::rand() % notSpecialGPs.size();
            GamePractice* toReturn = notSpecialGPs[randomIndex];

            notSpecialGPs.erase(notSpecialGPs.begin()+randomIndex);
            return toReturn;
    }



}

//this function creates a schedule through recursive methods
//it returns the first valid schedyle it encounters
Schedule Otree::runOtree(Node &node)
{
    if (node.schedule.assignments.size() == gamePractices.size()){  //if we have a full schedule, return it!
        return node.schedule;
    }


    if (node.remainingGPs.empty() && node.remainingSpecialGPs.empty() && node.evenings.empty()){ // this is just for error handling
        cout<<"fail to finD ANY BUT NOT FULL"<<endl;
        //exit(EXIT_FAILURE);
        return Schedule(); // supposed to be return NULL in java, could perhaps make some kind of "error" schedule
    }

    GamePractice* currentGP = getRandomIndex(node.remainingSpecialGPs, node.remainingGPs, node.evenings); //current gp is randomly selected
    vector<Slot *> openSlots; // vector of avaiable slots, needs to be initialized based off of gp TYPE




    if (currentGP->type ==GPType::GAME){ //if the gp is a game
        if((currentGP->age==15||currentGP->age == 16||currentGP->age==17||currentGP->age==19)&&(currentGP->underOver==0)){ //and is special
            openSlots = openGameSlotsAge;} //we set the open slots equal to open special slots
        else {
        openSlots = openGameSlots;} //otherwise, just set to general open slots
        }
        else {
                openSlots = openPracticeSlots; //if not game, set to general open practice slots
            }

    random_device rd; // Non-deterministic random number generator
    mt19937 g(rd());  // Mersenne Twister engine seeded with rd
    std::shuffle(openSlots.begin(), openSlots.end(), g); //shuffle open slots for leaf selector

    bool evening = false;
    std::string numStr = std::to_string(std::abs(currentGP->division)); 
    if (numStr[0] == '9'){
            evening = true;
    } //if it is an evening game. We get this so that we can sort slots by this method in leafselector


    stack<Slot *> slotStack = ls.getStack(currentGP, openSlots, evening); // get the slot stack from leaf selector

    int count = 0;

    while (!slotStack.empty()&&count<10)
    { //while we have slots left to go through, and the count is less than the max
        if(terminate) break; //if we have reached the time limit, return NULL

        Slot* trialSlot = slotStack.top(); // our trial slot, again, is from top, which is the "best" slots
        slotStack.pop(); //remove slot from stack

        if (node.schedule.IsValidPseudo(currentGP, trialSlot)) //if the schedule is valid with additional slot
        { 

            for (pair<GamePractice*, Slot*> assign : node.schedule.assignments){
            }

            node.schedule.AddAssignment(*currentGP, *trialSlot); // we are good to add it
            
            if (currentGP->type == GPType::GAME){ //if special gp, we remove the associated slot from the open list
                if((currentGP->underOver == 0)&&(currentGP->age==15||currentGP->age == 16||currentGP->age==17||currentGP->age==19)){
                    auto it = std::find(openGameSlotsAge.begin(), openGameSlotsAge.end(), trialSlot);
                    if (it != openGameSlotsAge.end()){
                        openGameSlotsAge.erase(it); // Remove the slot from the vector
                    }
                }
            }

            Node n = Node(node.schedule, node.remainingGPs, node.remainingSpecialGPs, node.evenings); //create new node for tree
        

            Schedule result = runOtree(n); // we can run the otree again

            
            if (result.getSize() != 0)
            {
                return result; // if schedule is not null, we have found a valid one, and can return!
            }
    
            node.schedule.RemoveAssignment(*currentGP); // remove assignment from schedule if invalid result found

    
            if (currentGP->type ==GPType::GAME){
                if((currentGP->underOver == 0)&&(currentGP->age==15||currentGP->age == 16||currentGP->age==17||currentGP->age==19)){
                    openGameSlotsAge.push_back(trialSlot);
                } //update slot from special slot list
            }
                            
        } 
        
        // count++; //can get rid of this if needed
    }


    std::string evStr = std::to_string(std::abs(currentGP->division)); //to get if 9 for division


    if((currentGP->underOver==0)&&(currentGP->age==15||currentGP->age == 16||currentGP->age==17||currentGP->age==19)&&(currentGP->type == GPType::GAME)){
        node.remainingSpecialGPs.push_back(currentGP);} //if special, add gp back to special list
    else if (evStr[0] == '9'){
    {
        node.evenings.push_back(currentGP);//if evening, add back to evening list
        }
    } 
    else {
        node.remainingGPs.push_back(currentGP); //otherwise, add back to regular list
    }
    return Schedule(); // AKA RETURN NULL (indicated failiure and need to check new leaf)
}