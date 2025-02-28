#pragma once
#include <vector>
#include <string>
// #include "environment.hpp"
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <map>

using namespace std;

enum GPType // Use if no need for difference between
{
    GAME = 0,
    PRACTICE = 1,
};

class Slot
{
public:
    GPType type;
    int gpMax;
    int gpMin;

    int day;
    int timeStart;
    int timeEnd;

    Slot(GPType t, int min, int max, int d, int intTime);
    ~Slot();
    int GetgpMax() { return gpMax; }
    int GetgpMin() { return gpMin; }

    bool CheckOverlap(const Slot &other);  // True if overlap
    bool CheckAdjacent(const Slot &other); // True if slots are directly adjacent

    // Comparisons use details of the slot
    bool operator==(Slot &other)
    {
        return (other.day == this->day &&
                other.timeStart == this->timeStart &&
                other.timeEnd == this->timeEnd);
    }
    bool operator<(Slot &other)
    {
        if (other.day == this->day &&
            other.timeStart < this->timeStart &&
            other.timeEnd <= this->timeStart)
        {
            return true;
        }
        else if (other.day < this->day)
        {
            return true;
        }
        return false;
    }
    bool slotCheck(GPType t, int d, int time)
    {
        if (t == this->type && d == this->day && time == this->timeStart)
        {
            return true;
        }
        return false;
    }

    std::string toString() const
    {
        std::ostringstream oss;
        oss << "T: " << (type == GAME ? "G" : "P") << ", "
            << "Max: " << gpMax << ", "
            << "Min: " << gpMin << ","
            << "Day: " << day << ","
            << "Start: " << timeStart << ","
            << "End: " << timeEnd << ", ";
        return oss.str();
    }
};
// Base Class for representing games and practices
class GamePractice
{
public:
    GPType type;
    std::string *league;
    int underOver;  // under == 0, over == 1;
    int age;        // represents U in the input.
    int tier;       // T1, T2, T3, etc 
    int division;   // DIV 01, DIV 02, etc.
    int special;    // 
    int team;
    vector<GamePractice *> notCompatible;
    map<Slot *, int> preferences;
    vector<GamePractice *> pair; 
    vector<Slot *> unwanted;
    string identifier;

    GamePractice(GPType t, std::string *l, int uo, int a, int tier, int d, int s, int intTeam, string id);
    ~GamePractice();
    bool operator==(GamePractice &other) // Unnecessary for teams
    {
        return (other.type == this->type &&
                other.league == this->league &&
                other.underOver == this->underOver &&
                other.age == this->age &&
                other.tier == this->tier &&
                other.division == this->division &&
                other.special == this->special &&
                other.team == this->team);
    }

    bool sameTeamNonPrac(GamePractice &other){
        return (
                other.league == this->league &&
                other.underOver == this->underOver &&
                other.age == this->age &&
                other.tier == this->tier &&
                other.division == this->division 
                //other.special == this->special
                );
    }

    bool sameTeamBothPrac(GamePractice &other){
        return (
                other.league == this->league &&
                other.underOver == this->underOver &&
                other.age == this->age &&
                other.tier == this->tier &&
                other.division == this->division &&
                other.special == this->special&&
                other.team == this->team);
    }



    bool operator<(const GamePractice &other) const
    {
        // Seems sus does this work?
        // Fixing the comparison order
        if (this->type != other.type)
            return this->type < other.type;
        if (*this->league != *other.league)
            return *this->league < *other.league; // Dereference to compare the actual string
        if (this->age != other.age)
            return this->age < other.age;
        if (this->tier != other.tier)
            return this->tier < other.tier;
        if (this->division != other.division)
            return this->division < other.division;

        return false; // If all fields are equal, return false.
    }

    void addNotWanted(Slot *slot)
    {
        unwanted.push_back(slot);
    }
    void addNotCompat(GamePractice *gp)
    {
        notCompatible.push_back(gp);
    }
    void addPref(Slot *slot, int pref)
    {
        preferences.insert(std::make_pair(slot, pref));
    }
    void addPair(GamePractice *gp)
    {
        pair.push_back(gp);
    }
    std::string toString() const
    {
        std::ostringstream oss;
        if (type == GAME)
        {
            oss << "T: " << (type == GAME ? "G" : "P") << ", "
                << "L: " << *league << ", "
                << "U/O: " << underOver << ", "
                << "A: " << age << ", "
                << "Tier: " << tier << ", "
                << "Div: " << division << ", "
                << "S: " << special << "\t";
        }
        else
        {
            oss << "T: " << (type == GAME ? "G" : "P") << ", "
                << "L: " << *league << ", "
               << "U/O: " << underOver << ", "
                << "A: " << age << ", "
                << "Tier: " << tier << ", "
               << "Div: " << division << ", "
                << "S: " << special << ", "
                << "Team: " << team << "\t";
               
        }
        return oss.str();
    }

    void printSelf(){
        cout<<this->toString()<<endl;
    }

    void printNotWanted() const
    {
        if (unwanted.size() != 0)
        {
            std::cout << "Not Wanted: {\n";

            for (const auto &slot : unwanted)
            {
                std::cout << "\t" << slot->toString() << ", " << std::endl;
            }
            std::cout << " \t} \n";
        }
        else
        {
            std::cout << std::endl;
        }
    }
    void printNotCompatible() const
    {
        if (notCompatible.size() != 0)
        {
            std::cout << "Not Compatible: {\n";
            for (const auto &gp : notCompatible)
            {
                std::cout << "\t" << gp->toString() << ", " << std::endl;
            }
            std::cout << " \t} \n";
        }
        else
        {
            std::cout << std::endl;
        }
    }
    void printPreferences() const
    {
        if (preferences.size() != 0)
        {
            std::cout << "Preferences: {\n";
            for (const auto &slot : preferences)
            {
                std::cout << "\t" << slot.first->toString() << " Pref: " << slot.second << std::endl;
            }
            std::cout << " \t} \n";
        }
        else
        {
            std::cout << std::endl;
        }
    }
    void printPair() const
    {
        if (pair.size() != 0)
        {
            std::cout << "Pair: {\n";
            for (const auto &gp : pair)
            {
                std::cout << "\t" << gp->toString() << ", " << std::endl;
            }
            std::cout << " } \n";
        }
        else
        {
            std::cout << std::endl;
        }
    }
};

// class Assignment
// {
// public:
//     bool empty;
//     GamePractice *gamePractice;
//     Slot *slot;
//     Assignment(GamePractice *gp, Slot *s); // Stores references to the original GamePractice and Slot objects

//     // True if references the same gamepractice and slot
//     bool operator==(const Assignment &other)
//     {
//         return other.gamePractice == gamePractice && other.slot == slot;
//     }
// };

class Schedule
{
private:
    // THIS CANNOT BE PUBLIC, IT MUST REMAIN IN SYNC WITH ASSIGNMENTS
    unordered_map<Slot *, vector<GamePractice *>> gamesInSlots; // Used for fast accessing, make sure this is synchronized
public:

    unordered_map<GamePractice *, Slot *> assignments;
    vector<GamePractice *> unassigned;
    int eval; // Computed by Evaluate
    bool isPartValid;
    bool isValid;


    void printUnassigned();
    Schedule();
    Schedule(const Schedule &partassign); // Allows specification of a partassign
    ~Schedule();
    int getSize();
    // int AddAssignment(Assignment &a);             // Adds an assignment if it doesn't yet exist
    int AddAssignment(GamePractice &gp, Slot &s); // Adds an assignment if it doesn't yet exist
    int RemoveAssignment(GamePractice &gp);       // Removes the assignment associated with a game or practice, if it exists
    // Slot GetAssignment(GamePractice &gp);   // Gets the slot assigned to a game or practice
    Slot *GetSlot(GamePractice &gp); // Gets the assignment associated with a game or practice CHECK FOR NULLPTR

    vector<GamePractice *> GetAssignments(Slot &s); // Gets the games and practices assigned to a slot
    bool IsAssigned(GamePractice &gp);              // Returns true if a game or practice is assigned
    size_t NumAssignedGames(Slot &s);               // Returns the number of games in a slot
    size_t NumAssignedPractices(Slot &s);           // Returns the number of practices in a slot
    bool SlotFullGames(Slot &s);                    // Returns true if the number of games in a slot equals the max assigned
    bool SlotFullPractices(Slot &s);                // Returns true if the number of practices in a slot equals the max assigned
    bool IsComplete();                              // Returns true if the schedule is complete
    vector<Slot*> GetUsedSlots();                   // Returns a vector of slots that have been assigned
    bool IsValidPseudo(GamePractice* gp, Slot* sl); // Check whether this schedule is valid

    // NEVER CALL FROM CONSTRAINTS OR EVAL
    bool IsPartValid(); // Check whether this partial schedule is valid
    bool IsValid();     // Check whether this schedule is valid
    int Evaluate();     // Evaluate the schedule and store the result in eval

    // Allows sorting by eval and comparing schedules easily
    /*bool operator<(const Schedule &other)
    {
        return other.eval < eval;
    }*/

   bool operator<(const Schedule &other) const
{
    return eval < other.eval;
}
    bool operator<=(const Schedule &other)
    {
        return other.eval <= eval;
    }
   /* bool operator==(const Schedule &other)
    {
        return other.eval == eval;
    }*/
void printSelf(){
            for (pair<GamePractice*, Slot*> assign : this->assignments){
            cout<<"GAME: "<<assign.first->toString()<<" SLOT: "<<assign.second->timeStart<<" day : "<<assign.second->day<<endl;
        }
}
    
    bool operator>=(const Schedule &other)
    {
        return other.eval >= eval;
    }
    bool operator>(const Schedule &other)
    {
        return other.eval > eval;
    }
};
