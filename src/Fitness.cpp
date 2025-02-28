



#include "Fitness.hpp"
#include <unordered_map>
#include <utility>
#include <unordered_set>
#include <algorithm>
#include "environment.hpp"
#include <cmath> // For std::ceil
#include <set>


using namespace std;


void Fitness::pauseAndWait(){
char input;
     std::cout << "Press 'c' to continue..." << std::endl;
     do {
         input = std::cin.get(); // Reads a single character from the input
    } while (input != 'c' && input != 'C'); // Allow both 'c' and 'C'
}
    
Fitness::Fitness(Schedule* toEval) : schedule(toEval){}


int Fitness::getMinFilledPenalty(Slot* slot){
    int minScore = 0;
    vector<GamePractice*> gpsInSlot = schedule->GetAssignments(*slot); //assignment vector from the specific slot (all associated assignments)


    
    int numGPsInSlot = gpsInSlot.size(); //number of gps in a slot is simply asignment vector in slot size
    if (gpsInSlot.size() < slot->gpMin){ //if there are less gps assigned to a slot than slotMin, then we have some penalties
 
            minScore+=(slot->gpMin - gpsInSlot.size())*penalties.gamemin; //penalty = (slot.GPMIN - slot.ACTUAL COUNT)*PENMIN

    }
    // cout<<"min score"<<minScore<<endl;
    //pauseAndWait();

    return minScore;
}

int Fitness::getPrefPenalty(Slot* slot){
int prefScore = 0;


 for (GamePractice* gp : schedule->GetAssignments(*slot)) {
    for (pair<Slot*, int> entry : gp->preferences) {
        Slot* prefSlot = entry.first; // Access the Slot* (key)
        int preference = entry.second; // Access the associated int (value)
        if (slot!=prefSlot){
            prefScore+=preference;}
        }
    }
        return prefScore;
}

int Fitness::getPairPenalty(Slot* slot) // TODO FIX LMAO
{
    vector<GamePractice*> visited;
    int pairScore = 0;

    // Use a set to track processed pairs
    set<pair<GamePractice*, GamePractice*>> processedPairs;

 for (GamePractice* wantedGP : schedule->GetAssignments(*slot)) {
    
    for (GamePractice* pairDesired : wantedGP->pair) {
        // Create a pair in consistent order (A, B)
            auto pairKey1 = make_pair(wantedGP, pairDesired);
            auto pairKey2 = make_pair(pairDesired, wantedGP);
        // Check if the pair has already been processed

            bool foundMatch = false;
            for (GamePractice* match  : schedule->GetAssignments(*slot)){
                if (match==pairDesired){
                    foundMatch = true;
                    }

                }
                if (!foundMatch){
           //     cout<<"found pair"<<endl;
                    
                pairScore += penalties.notpaired; // Add penalty only once
                processedPairs.insert(pairKey1);   // Mark the pair as processed
                processedPairs.insert(pairKey2);
            }
        //}
    }
            
        
    
}
return pairScore;

}


int Fitness::getSecPenalty(Slot* slot){
int secScore = 0;
        if (slot->type==GPType::GAME){
        for (size_t i = 0; i < schedule->GetAssignments(*slot).size(); ++i) { 
        GamePractice* currentGP = schedule->GetAssignments(*slot)[i]; //get a game practice that has the slot assignd to them

    // Check for other assignments with the same division, but only those after the current one

    for (size_t j = i+1; j < schedule->GetAssignments(*slot).size(); ++j) { 
        if (currentGP->age == schedule->GetAssignments(*slot)[j]->age) { 
            if (currentGP->tier == schedule->GetAssignments(*slot)[j]->tier){
            

            
            //if the division of a gp in the slot matches the division of another gp in the slot
            secScore += penalties.pen_section; // Add penalty only once for the pair 

            }
        }
        }
     }
    }
return secScore;
}

int Fitness::softConstrNewAssignment(Schedule schedule, pair<GamePractice*, Slot*> assign){}



int Fitness::softConstr(){ 
 
    int penalty = 0; 
    
    int minFilledFscore = 0;
    int prefFScore = 0;
    int pairPreFScore = 0;
    int secDiffPreFScore = 0;



vector<Slot> allSlots = slots; // Create a copy of all slots

for (Slot* slot : schedule->GetUsedSlots()) { // Iterate through all assignments in a schedule
    prefFScore += getPrefPenalty(slot) * weights.wPref;
    pairPreFScore += getPairPenalty(slot) * weights.wPair;
    secDiffPreFScore += getSecPenalty(slot) * weights.wSecDiff;
    minFilledFscore += getMinFilledPenalty(slot);

    // Use an equality check to find the slot in allSlots
    auto it = std::find_if(allSlots.begin(), allSlots.end(),
        [&](const Slot& s) { return s.toString()==slot->toString(); }); // Compare Slot object directly

    // Erase the slot if found
    if (it != allSlots.end()) {
        allSlots.erase(it);
    }
}
   // cout<<"all slots size: "<<allSlots.size()<<endl;
    for (Slot s : allSlots){
        minFilledFscore+=(s.gpMin)*penalties.gamemin; //penalty = (slot.GPMIN - slot.ACTUAL COUNT)*PENMIN
    }
    minFilledFscore = minFilledFscore*weights.wMinFilled;
    pairPreFScore = pairPreFScore/2;
    

    

    penalty = minFilledFscore + prefFScore + pairPreFScore + secDiffPreFScore;
   // cout<<"penalty: "<<penalty<<endl;
    return penalty;
}



//NOTE TO WHOEVER IS WORKING ON THIS: WE NEED SOME KIND OF LIST FROM GAMEPRACTICE OR SOMETHING, THAT RETURNS WHAT EACH SLOT WANTS TO BE PAIRED WITH
//THIS COULD EVEN BE A VERY BIG HASH SET OF GAMEPRACTICE, <VECTOR<GAMEPRACTICE> OR SOMETHING
//AS OF NOW, I ADDED A VECTOR "WANTTOBEPAIREDWITH" INTO THE GAME PRACTICE ELEMENTS
