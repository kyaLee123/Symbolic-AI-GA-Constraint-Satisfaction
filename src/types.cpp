#include <vector>
#include <string>
#include <typeinfo>
#include "types.hpp"
#include "constraints.hpp"
#include <algorithm>
#include <iostream>
#include "environment.hpp"
#include "Fitness.hpp"
// #include ".cmake/"

using namespace std;

GamePractice::GamePractice(GPType t, std::string *l, int uo, int a, int _tier, int d, int s, int intTeam, string id) : type(t), league(l), underOver(uo), age(a), tier(_tier), division(d), identifier(id)
{

    special = s;
    team = intTeam;
}

GamePractice::~GamePractice()
{
}

Slot::Slot(GPType t, int min, int max, int d, int intTime)
{
    type = t;
    day = d;
    timeStart = intTime;
    if (type == GAME)
    {
        if (day == 1) // day == Tuesday/thursday
        {
            int minutes = timeStart % 100;
            // DEBUG &&cout << minutes << std::endl;
            if (minutes == 30)
            {
                timeEnd = intTime + 170;
            }
            else
            {

                timeEnd = intTime + 130;
            }
        }
        else
        {
            timeEnd = intTime + 100;
        }
    }
    else // Type == PRACTICE
    {
        if (day == 2)
        {
            timeEnd = intTime + 200;
        }
        else
        {
            timeEnd = intTime + 100;
        }
    }
    gpMin = min;
    gpMax = max;
}
Slot::~Slot()
{
}

bool Slot::CheckOverlap(const Slot &other)
{
    return ((this->timeStart < other.timeEnd && other.timeStart < this->timeEnd)&&(this->day == other.day));
}

bool Slot::CheckAdjacent(const Slot &other)
{
    return (this->timeStart == other.timeEnd || other.timeStart == this->timeEnd);
}

// Assignment::Assignment(GamePractice *gp, Slot *s) // Stores references to the original GamePractice and Slot objects
//     : empty(false), gamePractice(gp), slot(s)
// {
// }

Schedule::Schedule() : eval(-1)
{

    for (GamePractice &gp : gamePractices)
    {
        unassigned.push_back(&gp);
    }
    // unassigned = gamePractices;
}

Schedule::Schedule(const Schedule &partassign) : eval(partassign.eval) // Allows specification of a partassign
{
    for (pair<GamePractice *, Slot *> a : partassign.assignments)
    {
        AddAssignment(*a.first, *a.second);
    }
    // unassigned = gamePractices;
    for (GamePractice &gp : gamePractices)
    {
        unassigned.push_back(&gp);
        ;
    }

    for (pair<GamePractice *, Slot *> a : partassign.assignments) // Remove assignments in partAssign
    {
        auto it = std::find(unassigned.begin(), unassigned.end(), a.first);
        if (it != unassigned.end())
        {
            unassigned.erase(it); // Remove the pointer from unassigned
        }
    }
}

Schedule::~Schedule()
{
}

void Schedule::printUnassigned()
{
    for (GamePractice *gp : unassigned)
    {
        gp->printSelf();
    }
}

int Schedule::AddAssignment(GamePractice &gp, Slot &s) // Adds an assignment if it doesn't yet exist
{
    // cout<<"in add assignment"<<endl;
    // cout<<"is assigned?:"<<IsAssigned(gp)<<endl;
    if (IsAssigned(gp))
    {
        return 1; // Error: Already assigned
    }

    if (assignments.find(&gp) == assignments.end())
    {
        assignments.insert({&gp, &s});

        // if(gamesInSlots.find(&s) == gamesInSlots.end())
        // {
        //     gamesInSlots.insert({&s, vector<GamePractice*>()});
        // }
        // gamesInSlots.at(&s).push_back(&gp);
        // cout<<"inserting NEW assignment"<<endl;
    }
    else
    {
        // cout<<"game is already assigned"<<endl;
        return -1;
    }

    // cout<<"hereeee"<<endl;

    if (gamesInSlots.find(&s) == gamesInSlots.end())
    {
        gamesInSlots.insert({&s, vector<GamePractice *>()});
    }
    gamesInSlots.at(&s).push_back(&gp);

    unassigned.erase(remove(unassigned.begin(), unassigned.end(), &gp), unassigned.end()); // Remove from unassigned list
    return 0;
}



int Schedule::RemoveAssignment(GamePractice &gp) // Removes the assignment associated with a game or practice, if it exists
{
    // cout << "In remove assignment in types" << endl;

    if (!IsAssigned(gp))
    {
        // cout << "GP is not assigned" << endl;
        return 1; // Error: Not assigned
    }

    // Store the associated slot before erasing
    if (assignments.find(&gp) == assignments.end())
    {
        //   cout<<"could not find assignment"<<endl;
        return 0;
    }
    Slot *associatedSlot = assignments.at(&gp);
    // cout << "Trying to erase from assignments" << endl;

    // Erase the assignment from the assignments map
    assignments.erase(&gp);
    // cout << "Done erasing" << endl;

    // Use the stored slot in the condition
    if (gamesInSlots.find(associatedSlot) == gamesInSlots.end())
    {
        //  cout << "Slot not found in gamesInSlots" << endl;
        return 0;
    }

    // Access and modify gamesInSlots using the stored slot
    vector<GamePractice *> &gps = gamesInSlots.at(associatedSlot);
    gps.erase(remove(gps.begin(), gps.end(), &gp), gps.end()); // Remove from gamesInSlots list

    // If the vector for the slot is empty, remove the slot from gamesInSlots
    if (gps.empty())
    {
        gamesInSlots.erase(associatedSlot);
    }

    // Add GP back to the unassigned list
    unassigned.push_back(&gp);

    return 0; // Success
}

Slot *Schedule::GetSlot(GamePractice &gp) // Gets the slot assigned to a game or practice
{
    if (assignments.find(&gp) != assignments.end())
    {
        //  cout<<"gp: "<<gp.toString()<<"\npaired to slot: "<<assignments.at(&gp)->toString()<<endl;
        return assignments.at(&gp);
    }
    return NULL;
}

vector<GamePractice *> Schedule::GetAssignments(Slot &s)
{
    // auto it = gamesInSlots.find(&s);
    // if (it != gamesInSlots.end()) {
    //     return it->second; // Key exists, return the associated vector
    // }
    vector<GamePractice *> result;
    for (pair<GamePractice *, Slot *> a : assignments)
    {
        if (a.second == &s)
        {
            result.push_back(a.first);
        }
    }
    return result; // Return an empty vector if the slot isn't in the map
}

bool Schedule::IsAssigned(GamePractice &gp) // Returns true if a game or practice is assigned
{
    if (GetSlot(gp) == NULL)
    {
        return false;
    }
    return true;
}

size_t Schedule::NumAssignedGames(Slot &s) // Returns the number of games or practices in a slot
{
    size_t counter;
    for (pair<GamePractice *, Slot *> a : assignments)
    {
        if (a.first->type == GAME)
            counter++;
    }
    if (gamesInSlots.find(&s) == gamesInSlots.end())
    {
        return 0;
    }
    return gamesInSlots.at(&s).size();
}

size_t Schedule::NumAssignedPractices(Slot &s) // Returns the number of games or practices in a slot
{
    size_t counter;
    for (pair<GamePractice *, Slot *> a : assignments)
    {
        if (a.first->type == PRACTICE)
            counter++;
    }
    return GetAssignments(s).size();
}

bool Schedule::SlotFullGames(Slot &s) // Returns true if the number of games in a slot equals the max assigned
{
    if (s.type != GAME)
        return false;
    return NumAssignedGames(s) >= s.gpMin;
}

bool Schedule::SlotFullPractices(Slot &s) // Returns true if the number of games or practices in a slot equals the max assigned
{
    if (s.type != PRACTICE)
        return false;
    return NumAssignedPractices(s) >= s.gpMax;
}

bool Schedule::IsComplete() // Returns true if the schedule is complete
{
    return assignments.size() >= gamePractices.size();
}

bool Schedule::IsPartValid() // Check whether this schedule is valid
{
    bool isPartValid = Constraints::PartConstr(this);
    return isPartValid;
}

bool Schedule::IsValid() // Check whether this schedule is valid
{
    // cout<<"about to enter constraints"<<endl;
    isValid = Constraints::Constr(this);
    cout << "is valid?: " << isValid << endl;
    return isValid;
}

bool Schedule::IsValidPseudo(GamePractice *gp, Slot *sl) // Check whether this schedule is valid
{
    // cout<<"about to enter constraints"<<endl;
    isValid = Constraints::PseudoConstr(this, gp, sl);
    // cout<<"is valid?: "<<isValid<<endl;
    return isValid;
}

int Schedule::Evaluate() // Evaluate the schedule and store the result in eval
{
    // eval = Constraints::GetEval(*this);
    Fitness fitness = Fitness(this);
    eval = fitness.softConstr();
    return eval;
}

int Schedule::getSize()
{
    return assignments.size();
}

vector<Slot *> Schedule::GetUsedSlots()
{
    vector<Slot *> usedSlots;
    for (pair<Slot *, vector<GamePractice *>> p : gamesInSlots)
    {
        usedSlots.push_back(p.first);
    }
    return usedSlots;
}