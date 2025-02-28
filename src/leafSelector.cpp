
/**/

#include "leafSelector.hpp"
#include "types.hpp"
#include <stack>
#include <random>


LeafSelector::LeafSelector(Schedule &A, Schedule &B)
:  selectorType("crossover"), parentA(A), parentB(B)
{}

LeafSelector::LeafSelector(Schedule &O)
:  selectorType("mutation"), originalMutSchedule(O){}

LeafSelector::LeafSelector()
:selectorType("initialize"){}

//an attempt at mutation leaf selector. It returns a stack, with the "worst" slot on bottom (AKA the slot that matches that in the original) and the 
//"best" slots on top (AKA all other slots)
stack<Slot*> LeafSelector::getMutationStack(GamePractice* emptyElement, vector<Slot*> &openSlotOptions, bool evening){
    stack<Slot*> mutStack;
    //if everything is running right, the empty element will be the single point mutation element that was stripped.
    //so we just have to find the empty element from the original schedule
    //we can always have a thing that if openSlotOptions is like really big, we just return at random to optimize
    //because if we have a lot of open slot options, then most likely we wont return the same slot as original
    //anyway, for now lets just do what we did in the doc
    vector<Slot*> bestSlots;


    for (pair<GamePractice*, Slot*> a : originalMutSchedule.assignments){

        GamePractice* gp = a.first;
        Slot* slot = a.second;

        if (gp==emptyElement){
            Slot* worstSlot = slot;
            //cout<<"found worst slot!"<<endl;
            mutStack.push(worstSlot); //make sure worst slot is on bottom of stack
            for (Slot* sl : openSlotOptions){
                if (sl != worstSlot){
                    if (evening){
                    if (sl->timeStart>=1800){
                        mutStack.push(sl);
                    }}
                    else {
                        mutStack.push(sl);
                    }
                }
            }
        //cout<<"size of mutstack: "<<mutStack.size()<<endl;
        
        return mutStack;

        }

    }
   // cout<<"never found worst slot"<<endl;
    return mutStack;
}
//only thing, might be better to use a queue for mutation and a stack for crossover. IDK if it would really affect run time tho,
//but you could add this for optimization if youd want to Im just personally too lazy     





stack<Slot*> LeafSelector::getCrossoverStack(GamePractice* emptyElement, vector<Slot*> &openSlotOptions, bool evening){

    stack<Slot*> crossoverStack;
    //vector<Slot*> evenings;
    //if everything is running right, the empty element will be the single point mutation element that was stripped.
    //so we just have to find the empty element from the original schedule
    //we can always have a thing that if openSlotOptions is like really big, we just return at random to optimize
    //because if we have a lot of open slot options, then most likely we wont return the same slot as original
    //anyway, for now lets just do what we did in the doc
    //Slot* bestSlotArray[2];
    //cout<<"in get crossover stack"<<endl;
    //pair<GamePractice*, Slot*> parentAAssignment = *next(parentA.assignmen;
    //pair<GamePractice*, Slot*> parentBAssignment = *next(parentB.assignments.begin(), i);
    Slot* parentASlot = parentA.GetSlot(*emptyElement);
    Slot* parentBSlot = parentB.GetSlot(*emptyElement);

    //cout<<"parent A index: "<<firstIndex<<endl;
    //cout<<"parent A slot: "<<parentASlot->toString()<<endl; 


//        cout<<"parent B indxe: "<<secondIndex<<endl;
//      cout<<"parent B slot: "<<parentBSlot->toString()<<endl; 
//

    //cout<<"parentA position: "<<firstIndex<<endl;
    //cout<<"parent B position"<<secondIndex<<endl;

    //if (bestSlotArray[0] != NULL && bestSlotArray[1] != NULL) {
        for (Slot* s : openSlotOptions){
            if (s!=parentBSlot&&s!=parentASlot){
                if (evening){
                    if (s->timeStart>=1800){
                        crossoverStack.push(s);
                    }}
                    else {
                        crossoverStack.push(s);
                    }
            }
        }
        
    std::random_device rd;  
    std::mt19937 gen(rd()); 
    std::uniform_int_distribution<int> st(0, 1);

    
    if (st(gen) == 0) 
    {
        
        crossoverStack.push(parentASlot);
        crossoverStack.push(parentBSlot);
    } 
    else 
    {
        crossoverStack.push(parentBSlot);
        crossoverStack.push(parentASlot);
    }

    return crossoverStack;
}





stack<Slot*> LeafSelector::getStack(GamePractice* emptyElement, //the empty GP from otree that needs to be assigned a stack of leaves to follow (AKA slots)
    vector<Slot*> &openSlotOptions, bool isEvening) //note that openSlotOptions is already pre-shuffled
{
    //cout<<"in get stack"<<endl;
    //cout<<"selector type is"<<selectorType<<endl;
    if (selectorType=="mutation"){
        return getMutationStack(emptyElement, openSlotOptions, isEvening);
    }
    else if (selectorType=="crossover"){
        return getCrossoverStack(emptyElement, openSlotOptions, isEvening);
    }
    else if (selectorType=="initialize"){

        //cout<<"open slot options: "<<openSlotOptions.size()<<endl;

        stack<Slot*> randomSlots;
        vector<Slot*> topStackSlots;
        for (Slot* s : openSlotOptions){ //note, slots are shuffled in otree
            if (isEvening){
                if (s->timeStart>=1800){
                randomSlots.push(s);
              //  cout<<"added a evening slot"<<endl;
                continue;
            }
            }
            else {
                if (s->timeStart>=1800){
                    randomSlots.push(s);
                } else {
                    topStackSlots.push_back(s);
                }
            }
        }
            
        if (!isEvening){
        for (Slot* s :topStackSlots){
            randomSlots.push(s);
        }
        
            }
//        cout<<"random slot size: "<<randomSlots.size()<<endl;

        return randomSlots;
    }

    stack<Slot*> temp;
    return temp;
}

LeafSelector::~LeafSelector()
{
}


