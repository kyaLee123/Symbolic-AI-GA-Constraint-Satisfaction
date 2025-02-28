
#include <vector>
#include "types.hpp"
#include "environment.hpp"
#include "constraints.hpp"
#include <map>
#include <algorithm>
#include <iostream>

using namespace std;

bool Constraints::Constr(Schedule* schedule)
{
    //cout<<"just entered constraints file"<<endl;
    return PartConstr(schedule); //&& schedule->IsComplete(); //NOTE: WHERE IS PART CONSTR?
}

void PAUSE(){
        char input;
    // std::cout << "Press 'c' to continue..." << std::endl;
    // do {
    //     input = std::cin.get(); // Reads a single character from the input
    // } while (input != 'c' && input != 'C'); // Allow both 'c' and 'C'
    

}

bool comp(const Slot *first, const Slot *second)
{

    if (second->day == first->day &&
        second->timeStart < first->timeStart &&
        second->timeEnd <= first->timeStart)
    {
        return true;
    }
    else if (second->day < first->day)
    {
        return true;
    }
    return false;
    /*
    if (first.day == second.day)
    {
        if (first.timeStart < second.timeStart)
        {
            return first.timeEnd < second.timeEnd;
        }
        return first.timeStart < second.timeStart;
    }
    return first.day < second.day;*/
}

void printSlot(Slot &s)
{
    cout << "\t Slot type: " << s.type << " day: " << s.timeStart << " length: " << s.timeEnd << "\n";
}
void printGP(GamePractice &gp)
{
    cout << "\t GamePratice type: " << gp.type << " league: " << gp.league << " age: " << gp.age << " team: " << gp.tier << " division: " << gp.division << "\n";
}


string btos(bool x) 
{ 
    if (x) 
        return "True"; 
    return "False"; 
} 

bool debug = false;
bool Constraints::PseudoConstr(Schedule* schedule, GamePractice * gamePractice, Slot * slot){
    // cout<<"in here"<<endl;
    //check slot max and #gp in slot
    // cout << schedule->GetAssignments(*slot).size() << endl;
    if (schedule->GetAssignments(*slot).size() >= slot->gpMax){
        if(debug) cout<<"constraint GPMAX violated"<<endl;
        return false;
    }



   // cout<<"in psdueo"<<endl;
    //cout<<"in psuedo constr"<<endl;
    //check league meeting
//CHECKED LEAGUE MEETING, GOOD TO GO
   // cout<<"slot type: "<<slot->type<<endl;
   // cout<<"day: "<<slot->day<<endl;
   // cout<<"time start: "<<slot->timeStart<<endl;

    if(slot->type == 0 && slot->day == 1 && slot->timeStart == 1100){//Game == 0, day =tues, time = 11
   // cout<<"cant pair that!"<<endl;
        if(debug) cout<<"constraint league meeting violated"<<endl;

        return false;
    }
    //cout<<"past here"<<endl;

    //check if unwanted
    for(Slot * check_slot : gamePractice->unwanted){
        if(check_slot == slot){
       //     cout<<"unwanted reached"<<endl;
            if(debug) cout<<"constraint unwanted violated"<<endl;

            return false;

        }
    }   // cout<<"past here1"<<endl;


    //check if evening
    //get the left most digit
    int left_digit = gamePractice->division;
    //cout<<"in evening"<<endl;
    while (left_digit > 9) {
        left_digit /= 10;
    }
    //cout<<"past the div"<<endl;
    if (left_digit == 9) {
        if(slot->timeStart < 1800){
          //  cout<<"invalid: not evening"<<endl;
            if(debug) cout<<"constraint evening slot violated "<<endl;
//            cout<<"evening violated"<<endl;

            return false;

        }
        //cout<<"passed the evening"<<endl;
    }
//cout<<"past here2"<<endl;

    //check if overlapping 15,16,17,19 games
    int gp_age = gamePractice->age;
    bool under_lapping = false;
    if(slot->type == 0 && gamePractice->underOver == 0){//Game
        if (gp_age == 15 || gp_age == 16 || gp_age == 17 || gp_age == 19) {
            under_lapping = true;
        }
    }
   // cout << "\tStarting\n";
    //slot overlap
    for(Slot * check_slot : schedule->GetUsedSlots()){
        //cout << "\t way before";
        if(check_slot->CheckOverlap(*slot)){

            //cout << "\t Before";
            // (this loops based on gpMax, so maybe like 5 times)
            for(GamePractice* check_gamePractice : schedule->GetAssignments(*check_slot)){
  
                //checks if overlaps with own team, check if that gamePractice is inside
                if ((check_gamePractice->type==GPType::GAME)||(gamePractice->type==GPType::GAME)){
                   // cout<<"one game one "
                    if(check_gamePractice->sameTeamNonPrac(*gamePractice)){
                        return false;

                }
                }
                else if(check_gamePractice->sameTeamBothPrac(*gamePractice)){
                    //cout<<check_gamePractice->toString()<<endl;
                    //cout<<gamePractice->toString()<<endl;
                    //cout<<"issue here d"<<endl;
                    if(debug) cout << "constraint self overlap violated\n";
                    return false;
                }



//cout<<"past here4"<<endl;


                //check if overlapping 15,16,17,19 games
                if(under_lapping && check_slot->type == 0){
                    //check if team is within these age
                    if(check_slot->type == 0&& gamePractice->underOver == 0){//Game and under
                        gp_age = check_gamePractice->age;
                        if (gp_age == 15 || gp_age == 16 || gp_age == 17 || gp_age == 19) {
                            if(debug) cout << "constraint 15-19 overlap violated\n";
                            return false;
                        }
                    }

                    
                }
            }

        }
    }//cout<<"past here5"<<endl;


    //slot not compatiable





    for(GamePractice * notCompat : gamePractice->notCompatible){
        //cout<<"looping"<<endl;
      //  cout<<"NOT COMPAT ELEMENT: "<<endl;
      //  cout<<notCompat->toString()<<endl;


        Slot * check_slot = schedule->GetSlot(*notCompat);
        //cout<<"past check slot"<<endl;
        //cout<<"check slot: "<<check_slot<<endl;
        if (check_slot==NULL){
      //      cout<<"it seems this slot is not in schedule"<<endl;
            continue;
                                            // cout<<"issue here e"<<endl;

        }

        if(check_slot->CheckOverlap(*slot)){
         //   cout<<"return false"<<endl;
            //cout<<"there was not compatiable"<<endl;
             //                                        cout<<"issue here f"<<endl;
            if(debug) cout << "constraint not compatiable violated\n";
            return false;

        }
    }


//cout<<"past here6"<<endl;

    
    
    // cout<<"leaving"<<endl;
    //cout<<"passed constr"<<endl;
    return true;
}




bool Constraints::PartConstr(Schedule*schedule){
    bool debug = false;
    //I liked the gamesInSlots because it has a contains function, so i'll recreate it
    unordered_map<Slot *, vector<GamePractice *>> gamesInSlots;
    for(Slot * slot : schedule->GetUsedSlots()){ 
        gamesInSlots.insert({slot, schedule->GetAssignments(*slot)});
    }

    //consider this like an inverse of the above the code
    unordered_map<GamePractice * , vector<Slot*>> comparing;

    bool is_valid = true;

    //cout << "\t schedule->assignments size" << schedule->assignments.size();
    //cout << "\t schedule->gamesInSlots size" << gamesInSlots.size();

    //populate the comparing map
    //loop through existing assignments
    
    for (pair<GamePractice *, Slot *> assign : schedule->assignments) {

        //get game practice
        GamePractice* gamePractice = assign.first;

        //if GamePractice does not exist, make a new list and key
        if(!comparing.contains(gamePractice)){
            comparing.insert({gamePractice, {}});
        }
        //add to comparing list
        comparing.at(gamePractice).push_back(assign.second);
    }
    
  

    //sort all slots in comparing
    //this is to migitate the amount of comparisons and make comparing easier
    for (pair<GamePractice * , vector<Slot*>> pair : comparing) {
        sort(pair.second.begin(), pair.second.end());
    }

    //double check
    if(debug) cout << "Printing the sorted comparing map\n";
    for (pair<GamePractice * , vector<Slot*>> pair : comparing) {
       // cout << "\t" << pair.first->toString() << "\n";
        for(Slot* slot : pair.second){
        //    cout << "\t\t" << slot->toString() << "\n";
        }

    }

    //After this be ordered as (day, time, length) (with a full schedule)
    //(0,8,1)(0,8,1)(0,9,1)(0,9,1)...(0,20,1)(0,20,1)
    //(1,8,1) (1,8,1.5) (1,9,1) (1,9.5,1.5) (1,10,1) (1,11,1) (1,11,1.5) ...
    //...
    //(4,8,1) (4,8,2) (4,9,1) (4,10,1) (4,10,2) (4,11,1) 

    //VIOLATION O: CHECK IF SLOT IS OVERFILLED
    //check if a division is practising and gaming at the same time
    if(debug) cout << "Checking Max Hard Constraint A\n";
   // is_valid = constraint_max(gamesInSlots);
    if(!is_valid) return is_valid;
    if(debug) cout << "Done Checking Max  Hard Constraint A\n";


    //VIOLATION A: A DIVISION CANNOT HAVE PRACTICE AND GAMES OVERLAP
    //check if a division is practising and gaming at the same time
    if(debug) cout << "Checking Overlap Hard Constraint A\n";
   // is_valid = constraint_overlap(comparing);
    if(!is_valid) return is_valid;
    if(debug) cout << "Done Checking Overlap Hard Constraint A\n";


    //VIOLATION B: PLAYERS MAY BELONG IN MULTIPLE DIVISIONS, DEFINED BY NOTCOMPATIBLE
    //CANNOT HAVE THESE TEAMS HAVE OVERLAPPING PRACTICE AND GAMES
    if(debug) cout << "Checking Hard Constraint B\n";
    //is_valid = constraint_notcompatible(comparing);
    if(!is_valid) return is_valid;
    if(debug) cout << "Done Checking Hard Constraint B\n";

    
    //VIOLATION C: DIVISIONS ALREADY HAVE PRE ARRANGEMENTS FOR SLOTS
    //CONSIDER THIS A RESERVATION AND MAKE SURE THE RESERVATION EXISTS
    //check partial assignment
    if(debug) cout << "Checking Hard Constraint C\n";
   // is_valid = constraint_partialassignment(gamesInSlots);
    if(!is_valid) return is_valid;
    if(debug) cout << "Done Checking Hard Constraint C\n";
    

    //VIOLATION D: JUST SOME TEAM HAVE UNWANTED TIMES
    if(debug) cout << "Checking unwanted Hard Constraint D\n";
    //is_valid = constraint_unwanted(comparing);
    if(!is_valid) return is_valid;
    if(debug) cout << "Done Checking unwanted Hard Constraint D\n";

    //City of Calgary HARD CONSTRAINTS

    //Constraint E: JUST SOME TEAM HAVE UNWANTED TIMES
    //All divisions starting with 9, are evening and scheduled into evening slots
    //18:00 or later are evening slots
    if(debug) cout << "Checking evening Hard Constraint E\n";
    //is_valid = constraint_evening(comparing);
    if(!is_valid) return is_valid;
    if(debug) cout << "Done Checking evening Hard Constraint E\n";

    //All games in all tiers of the U15/U16/U17/U19 level must be scheduled into non-overlapping time slots
    //loop through and gather u15, u16, u17 and u19 and get games

    if(debug) cout << "Checking U15/U16/U17/U19 Hard Constraint F\n";
    //is_valid = constraint_overlapping_under(comparing);
    if(!is_valid) return is_valid;
    if(debug) cout << "Done Checking U15/U16/U17/U19 Hard Constraint F\n";

    if(debug) cout << "Checking League Meeting Hard Constraint G\n";
    //is_valid = constraint_league_meeting(gamesInSlots);
    if(!is_valid) return is_valid;
    if(debug) cout << "Done Checking League Meeting Hard Constraint G\n";

    if(debug) cout << "Checking Special Booking Hard Constraint H\n";
    //is_valid = constraint_special_booking(gamesInSlots);
    if(!is_valid) return is_valid;
    
    if(debug) cout << "-------------------Congrats Nothing Violated!--------------------\n";


    return is_valid;
}