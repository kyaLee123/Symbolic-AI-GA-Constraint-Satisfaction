#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm> // Include algorithm for std::all_of
#include "types.hpp"
#include "environment.hpp"

const int FLAG_EOF = -3;
const int FLAG_DNF = -1;
const int FLAG_FINISH = 0;

class Parser
{
public:
    Parser(std::string &fd);
    int parse();

private:
    std::string filename;
    std::ifstream file;
    std::streampos start_position;
    std::streampos next_position;

    std::string operation[11] = {
        "Name:", "Game slots:", "Practice slots:", "Games:",
        "Practices:", "Not compatible:", "Unwanted:",
        "Preferences:", "Pair:", "Partial assignments:", "EOF"};

    int op_count = 0;

    std::string name;
    int flag = 0;
    int gameslots_count = 0;
    int practiceslots_count = 0;
    int games_count = 0;
    int practices_count = 0;
    int not_compatible_count = 0;
    int unwanted_count = 0;
    int preferences_count = 0;
    int pair_count = 0;
    int part_ass_count = 0;

    std::vector<std::string> strip_string;

    bool find_operation_pos(std::ifstream &file, const std::string &operation, std::streampos &position);
    bool count_lines(std::ifstream &file, const std::string &next_category, int &count, std::streampos &startPosition, std::streampos &endPosition);
    bool get_name(std::ifstream &file);
    std::string trim(const std::string &str);
    void split_trim(const std::string &input, std::vector<std::string> &result);
    void split_trim_space(const std::string &input, std::vector<std::string> &result);
    int day_convert(const std::string &x);
    int time_conversion(const std::string &x);
    int min_max_conversion(const std::string &x);
    int slot_factory(std::ifstream &file, std::string next_category, std::streampos &startPosition, GPType type);
    std::string *find_league_or_add(std::string &str);
    int gp_factory(std::ifstream &file, std::string category, std::string next_category, std::streampos &startPosition, GPType type);
    bool printGamePractices();
    bool printSlots();
    int compatibility_factory(std::ifstream &file, std::string next_category, std::streampos &startPosition);
    Slot *find_special_slot();
    int unwanted_factory(std::ifstream &file, std::string next_category, std::streampos &startPosition);
    int preferences_factory(std::ifstream &file, std::string next_category, std::streampos &startPosition);
    int pair_factory(std::ifstream &file, std::streampos &startPosition);
    int partialAssignment_factory(std::ifstream &file, std::streampos &startPosition);
    GamePractice *build_gamePractice(std::string &strIn);
    GamePractice *gp_find(std::string &strIn);
    Slot *slot_find(GPType type, std::string &strDay, std::string &strTime);
    GamePractice *create_special_practice(int a, int div);
};