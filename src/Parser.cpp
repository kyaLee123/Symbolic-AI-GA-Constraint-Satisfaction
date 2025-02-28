#include "Parser.hpp"
#include <format>

Parser::Parser(std::string &fd)
    : filename(fd)
{
}

int Parser::parse()
{
    std::ifstream file(filename);
    if (!file)
    {
        std::cerr << "Error: Failed to open file " << filename << ".\n";
        return -1;
    }

    if (!get_name(file))
    {
        std::cerr << "Error: Failed to retrieve name from file " << filename << ".\n";
        file.close();
        return -1;
    }

    if (!find_operation_pos(file, operation[1], start_position))
    {
        std::cerr << "Error: Failed to find operation position for " << operation[1] << " in file " << filename << ".\n";
        file.close();
        return -1;
    }

    if (count_lines(file, operation[2], gameslots_count, start_position, next_position) != true)
    {
        std::cerr << "Error: Failed to count lines for " << operation[1] << " in file " << filename << ".\n";
        file.close();
        return -1;
    }
    slots.reserve(gameslots_count);
    gameslots.reserve(gameslots_count);

    if (slot_factory(file, operation[2], start_position, GAME) != 0)
    {
        std::cerr << "Error: Failed to create slots for " << operation[1] << " in file " << filename << ".\n";
        return -1;
    }

    start_position = next_position;
    if (count_lines(file, operation[3], practiceslots_count, start_position, next_position) != true)
    {
        std::cerr << "Error: Failed to count lines for " << operation[2] << " in file " << filename << ".\n";
        file.close();
        return -1;
    }
    slots.reserve(gameslots_count + practiceslots_count);
    practiceslots.reserve(practiceslots_count);

    if (slot_factory(file, operation[3], start_position, PRACTICE) != 0)
    {
        std::cerr << "Error: Failed to create slots for " << operation[2] << " in file " << filename << ".\n";
        file.close();
        return -1;
    }
    DEBUG &&printSlots();
    start_position = next_position;
    if (count_lines(file, operation[4], games_count, start_position, next_position) != true)
    {
        std::cerr << "Error: Failed to count lines for " << operation[3] << " in file " << filename << ".\n";
        file.close();
        return -1;
    }
    gamePractices.reserve(games_count);
    games.reserve(games_count);
    if (gp_factory(file, operation[3], operation[4], start_position, GAME) != 0)
    {
        std::cerr << "Error: Failed to create slots for " << operation[3] << " in file " << filename << ".\n";
        file.close();
        return -1;
    }

    start_position = next_position;
    if (count_lines(file, operation[5], practices_count, start_position, next_position) != true)
    {
        std::cerr << "Error: Failed to count lines for " << operation[4] << " in file " << filename << ".\n";
        file.close();
        return -1;
    }
    gamePractices.reserve(games_count + practices_count);
    practices.reserve(practices_count);

    if (gp_factory(file, operation[4], operation[5], start_position, PRACTICE) != 0)
    {
        std::cerr << "Error: Failed to create slots for " << operation[4] << " in file " << filename << ".\n";
        file.close();
        return -1;
    }
    DEBUG &&printGamePractices();
    // ================================================= Compatibility functions
    start_position = next_position;
    if (count_lines(file, operation[6], not_compatible_count, start_position, next_position) != true)
    {
        std::cerr << "Error: Failed to count lines for " << operation[5] << " in file " << filename << ".\n";
        file.close();
        return -1;
    }
    if (compatibility_factory(file, operation[6], start_position) != 0)
    {
        std::cerr << "Error: Failed to create compatibilities for " << operation[5] << " in file " << filename << ".\n";
        file.close();
        return -1;
    }

    // ======================================================== Unwanted factory
    start_position = next_position;
    if (count_lines(file, operation[7], unwanted_count, start_position, next_position) != true)
    {
        std::cerr << "Error: Failed to count lines for " << operation[6] << " in file " << filename << ".\n";
        file.close();
        return -1;
    }

    if (unwanted_factory(file, operation[7], start_position) != 0)
    {
        std::cerr << "Error: Failed to create unwanted for " << operation[6] << " in file " << filename << ".\n";
        file.close();
        return -1;
    }
    // ===================================================== Preferences Factory
    start_position = next_position;
    if (count_lines(file, operation[8], preferences_count, start_position, next_position) != true)
    {
        std::cerr << "Error: Failed to count lines for " << operation[7] << " in file " << filename << ".\n";
        file.close();
        return -1;
    }
    if (preferences_factory(file, operation[8], start_position) != 0)
    {
        std::cerr << "Error: Failed to create preferences for " << operation[7] << " in file " << filename << ".\n";
        file.close();
        return -1;
    }
    // ============================================================ Pair Factory
    start_position = next_position;
    if (count_lines(file, operation[9], pair_count, start_position, next_position) != true)
    {
        std::cerr << "Error: Failed to count lines for " << operation[8] << " in file " << filename << ".\n";
        file.close();
        return -1;
    }
    int pair_check = pair_factory(file, start_position);
    if (pair_check == -1)
    {
        std::cerr << "Error: Failed to create pair for " << operation[8] << " in file " << filename << ".\n";
        file.close();
        return -1;
    }
    else if (pair_check == -2)
    {
        std::cout << "detected invalid input in pair factory.\n";
        file.close();
        return -2;
    }
    std::cout << "Past pair factory\n";
    // ============================================= partial assignments factory
    start_position = next_position;
    // if (count_lines(file, operation[9], operation[10], part_ass_count, start_position, next_position) < 0)
    // {
    //     std::cerr << "Error: Failed to count lines for " << operation[9] << " in file " << filename << ".\n";
    //     return -1;
    // }

    int part_check = partialAssignment_factory(file, start_position);
    if (part_check == -1)
    {
        std::cerr << "Error: Failed to create partial assignments for " << operation[9] << " in file " << filename << ".\n";
        file.close();
        return -1;
    }
    if (part_check == -2)
    {
        std::cout << "Exiting out of parser having found invalid schedule.\n";
        file.close();
        return -2;
    }

    file.close();
    return 0;
}

bool Parser::find_operation_pos(std::ifstream &file, const std::string &operation, std::streampos &position)
{
    std::string line;
    while (std::getline(file, line))
    {
        if (line.find(operation) != std::string::npos)
        {
            position = file.tellg();
            return true;
        }
    }
    std::cerr << "EOF reached before " << operation << " found." << std::endl;
    return false;
}

bool Parser::count_lines(std::ifstream &file, const std::string &next_category, int &count, std::streampos &startPosition, std::streampos &endPosition)
{
    file.seekg(startPosition);
    std::string line;
    while (std::getline(file, line))
    {
        if (line.find(next_category) != std::string::npos)
        {
            endPosition = file.tellg();
            // DEBUG &&std::cout << "count: " << count << "\t" << category << std::endl;
            return true;
        }
        else if (!line.empty())
        {
            count++;
        }
    }
    if (file.eof())
    {
        std::cerr << "End of file reached before " << next_category << " found\n";
        return false;
    }
    else
    {
        std::cerr << "There was an error reading from the file during " << next_category << " search.\n";
        return false;
    }
}

bool Parser::get_name(std::ifstream &file)
{
    std::string line;
    while (std::getline(file, line))
    {
        if (line.find("Name:") != std::string::npos)
        {
            while (std::getline(file, line))
            {
                if (!line.empty() && (line.find(operation[1]) == std::string::npos))
                {
                    name = line;
                    // DEBUG &&std::cout << "Name: " << name << std::endl;
                    return true;
                }
                if (line.find(operation[1]) != std::string::npos)
                {
                    std::cerr << "Found " << operation[1] << " before finding name" << std::endl;
                    return false;
                }
            }
        }
    }
    std::cerr << "EOF reached before \"Name:\" found\n";
    return false;
}

std::string Parser::trim(const std::string &str)
{
    size_t first = str.find_first_not_of(' ');
    if (first == std::string::npos)
        return "";
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, last - first + 1);
}

void Parser::split_trim(const std::string &input, std::vector<std::string> &result)
{
    std::stringstream ss(input);
    std::string item;

    while (std::getline(ss, item, ','))
    {
        result.push_back(trim(item));
    }
}

void Parser::split_trim_space(const std::string &input, std::vector<std::string> &result)
{
    std::stringstream ss(input);
    std::string item;

    while (std::getline(ss, item, ' '))
    {
        result.push_back(trim(item));
    }
}

int Parser::day_convert(const std::string &x)
{
    std::string trimmed = trim(x);
    std::unordered_map<std::string, int> day_map = {
        {"MO", 0}, {"TU", 1}, {"FR", 2}};

    auto it = day_map.find(trimmed);
    if (it != day_map.end())
    {
        return it->second;
    }
    else
    {
        std::cerr << "There was an issue converting days to integers." << std::endl;
        return -1;
    }
}

int Parser::time_conversion(const std::string &x)
{
    std::string trimmed = trim(x);

    // Remove the colon character
    trimmed.erase(std::remove(trimmed.begin(), trimmed.end(), ':'), trimmed.end());

    // Check if the trimmed string has exactly 4 characters and all are digits
    if ((trimmed.size() > 4 || trimmed.size() < 3) || !std::all_of(trimmed.begin(), trimmed.end(), ::isdigit))
    {
        std::cerr << trimmed << " is an invalid time format." << std::endl;
        return -1;
    }

    // Convert the concatenated string to an integer
    int time = std::stoi(trimmed);

    // Extract hours and minutes
    int hours = time / 100;
    int minutes = time % 100;

    // Validate hours and minutes
    if (hours < 0 || hours > 23 || minutes < 0 || minutes > 59)
    {
        std::cerr << trimmed << " is an invalid time format. Hours: " << hours << ", Minutes: " << minutes << std::endl;
        return -1;
    }

    return time;
}

int Parser::min_max_conversion(const std::string &x)
{
    std::string trimmed = trim(x);
    if (trimmed.empty() || !std::all_of(trimmed.begin(), trimmed.end(), ::isdigit))
    {
        std::cerr << trimmed << " is an invalid min_max format." << std::endl;
        return -1;
    }
    int min_max = std::stoi(trimmed);

    return min_max;
}

int Parser::slot_factory(std::ifstream &file, std::string next_category, std::streampos &startPosition, GPType type)
{

    int day;
    int time;
    int gp_max;
    int gp_min;

    file.seekg(startPosition);
    std::string line;

    while (true)
    {
        std::vector<std::string> vec_temp;
        if (!std::getline(file, line))
        {
            if (file.eof())
            {
                std::cerr << "End of file reached before " << next_category << " found during obj factory\n";
                return int(FLAG_EOF);
            }
            else
            {
                std::cerr << "There was an error reading from the file during the slot factory .\n";
                return FLAG_DNF;
            }
        }
        if (line.find(next_category) != std::string::npos)
        {
            DEBUG &&std::cout << "Finished Creating Slots " << std::endl;
            return 0;
        }

        // Remove Windows-style carriage return if present
        if (!line.empty() && line.back() == '\r')
        {
            line.pop_back();
        }

        if (!line.empty())
        {
            split_trim(line, vec_temp);
            day = day_convert(vec_temp[0]);
            time = time_conversion(vec_temp[1]);
            gp_max = min_max_conversion(vec_temp[2]);
            gp_min = min_max_conversion(vec_temp[3]);

            // DEBUG &&std::cout << "day: " << day << " time: " << time
            //                   << " gp_max: " << gp_max << " gp_min: "
            //                   << gp_min << std::endl;

            if (day == -1 || time == -1 || gp_max == -1 || gp_min == -1)
            {
                std::cerr << "An error occurred during the conversion of line to obj in games+practices line: " << line << std::endl;
                return -1;
            }
            Slot temp(type, gp_min, gp_max, day, time);
            slots.push_back(temp);
            if (type == GAME)
            {
                gameslots.push_back(&slots.back());
            }
            else
            {
                practiceslots.push_back(&slots.back());
            }
            // DEBUG &&std::cout << temp.toString() << std::endl;
        }
    }

    return 0;
}

std::string *Parser::find_league_or_add(std::string &str)
{
    // Check if the string is already in the vector
    auto it = std::find(leagues.begin(), leagues.end(), str);
    if (it != leagues.end())
    {
        // If found, return a pointer to the string
        return &(*it);
    }
    else
    {
        leagues.push_back(str);
        return &leagues.back();
    }
}

// We schedule only games that occur in the list after Games:
// Exception: if we have under Games: games starting with CMSA U12T1 or
// CMSA U13T1, we have to schedule CMSA U12T1S (resp. CMSA U13T1S) in the
// indicated slots (note, they are scheduled into practice slots) and have to add all
// the constraints regarding them that are given in the problem description.
int Parser::gp_factory(std::ifstream &file, std::string category, std::string next_category, std::streampos &startPosition, GPType type)
{

    file.seekg(startPosition);
    std::string line;

    while (true)
    {
        std::vector<std::string> vec_temp;
        if (!std::getline(file, line))
        {
            if (file.eof())
            {
                std::cerr << "End of file reached before " << next_category << " found during obj factory\n";
                return int(FLAG_EOF);
            }
            else
            {
                std::cerr << "There was an error reading from the file during " << category << " factory .\n";
                return FLAG_DNF;
            }
        }
        if (line.find(next_category) != std::string::npos)
        {
            DEBUG &&std::cout << "Finished Creating: " << category << std::endl;
            return 0;
        }

        // Remove Windows-style carriage return if present
        if (!line.empty() && line.back() == '\r')
        {
            line.pop_back();
        }

        if (!line.empty())
        {
            GamePractice *temp = build_gamePractice(line);
            // DEBUG &&std::cout << temp.toString() << std::endl;
            gamePractices.push_back(*temp);
            if (type == GAME)
            {
                games.push_back(&gamePractices.back());
            }
            else
            {
                practices.push_back(&gamePractices.back());
            }
        }
    }
    return 0;
}

bool Parser::printSlots()
{
    std::cout << "Slots: \n";
    for (const auto &slot : slots)
    {
        std::cout << slot.toString() << std::endl;
    }
    return true;
}

bool Parser::printGamePractices()
{
    std::cout << "Game Practices: \n";
    for (const auto &gp : gamePractices)
    {
        std::cout << gp.toString() << std::endl;
    }
    return true;
}

int Parser::compatibility_factory(std::ifstream &file, std::string next_category, std::streampos &startPosition)
{
    file.seekg(startPosition);
    std::string line;

    while (true)
    {
        if (!std::getline(file, line))
        {
            if (file.eof())
            {
                std::cerr << "End of file reached before " << next_category << " found during compatibility factory\n";
                return int(FLAG_EOF);
            }
            else
            {
                std::cerr << "There was an error reading from the file during not compatible factory.\n";
                return FLAG_DNF;
            }
        }
        if (line.find(next_category) != std::string::npos)
        {
            if (DEBUG)
            {
                std::cout << "\nObjects + Their compatabilities:\n";
                for (auto &gp : gamePractices)
                {
                    if (!gp.notCompatible.empty())
                    {
                        std::cout << "GAMEPRACTICE OBJ:\t" << gp.toString() << "\n";
                        gp.printNotCompatible();
                    }
                }

                std::cout << "Finished Creating Compatibilities\n";
            }
            return 0;
        }

        // Remove Windows-style carriage return if present
        if (!line.empty() && line.back() == '\r')
        {
            line.pop_back();
        }

        if (!line.empty())
        {
            std::vector<std::string> vec_temp;
            split_trim(line, vec_temp);

            GamePractice *f_gp;
            GamePractice *s_gp;

            f_gp = gp_find(vec_temp[0]);
            s_gp = gp_find(vec_temp[1]);
            if (f_gp == nullptr || s_gp == nullptr)
            {
                std::cerr << "Error: Could not find f_gp or S_GP\n";
                return -1;
            }
            f_gp->addNotCompat(s_gp);
            s_gp->addNotCompat(f_gp);

            // if (DEBUG)
            // {
            //     f_gp->printNotCompatible();
            //     s_gp->printNotCompatible();
            // }
        }
    }

    return 0;
}

int Parser::unwanted_factory(std::ifstream &file, std::string next_category, std::streampos &startPosition)
{
    file.seekg(startPosition);
    std::string line;

    while (true)
    {
        if (!std::getline(file, line))
        {
            if (file.eof())
            {
                std::cerr << "End of file reached before " << next_category << " found during compatibility factory\n";
                return int(FLAG_EOF);
            }
            else
            {
                std::cerr << "There was an error reading from the file during not unwanted factory.\n";
                return FLAG_DNF;
            }
        }
        if (line.find(next_category) != std::string::npos)
        {
            if (DEBUG)
            {
                std::cout << "NotWanted: \n";
                for (auto &gp : gamePractices)
                {
                    if (!gp.unwanted.empty())
                    {
                        std::cout << gp.toString() << "\n";
                        gp.printNotWanted();
                    }
                }
                std::cout << "Finished Creating Unwanted \n";
            }

            return 0;
        }

        // Remove Windows-style carriage return if present
        if (!line.empty() && line.back() == '\r')
        {
            line.pop_back();
        }

        if (!line.empty())
        {
            std::vector<std::string> vec_temp;
            split_trim(line, vec_temp);

            GamePractice *f_gp;
            Slot *slot;

            f_gp = gp_find(vec_temp[0]);

            slot = slot_find(f_gp->type, vec_temp[1], vec_temp[2]);

            if (f_gp == nullptr || slot == nullptr)
            {
                std::cout << "ERROR ERROR ERROR\n";
                std::cerr << "Error: Could not find f_gp or Slot\n";
                return -1;
            }
            f_gp->addNotWanted(slot);
        }
    }

    return 0;
}

GamePractice *Parser::build_gamePractice(std::string &strIn)
{
    std::string str_league;
    std::string unit_tier;
    GPType type;

    int division;

    std::string *league;
    int underOver = 0;
    int age = 0;
    int tier = 0;
    int special = 0;
    int team = -1;

    std::vector<std::string> vec_temp;

    if (strIn.find("OPN") != std::string::npos || strIn.find("PRC") != std::string::npos)
    {
        type = PRACTICE;
    }
    else
    {
        type = GAME;
    }
    split_trim_space(strIn, vec_temp);
    league = find_league_or_add(vec_temp[0]);
    // under over designator
    if (vec_temp[1][0] == 'U')
    {
        underOver = 0;
    }
    else
    {
        underOver = 1;
    }
    // Age, tier
    try
    {
        if (vec_temp[1].length() < 4)
        {
            age = std::stoi(vec_temp[1].substr(1, 2));
            tier = 0;
        }
        else
        {
            age = std::stoi(vec_temp[1].substr(1, 2));
            tier = std::stoi(vec_temp[1].substr(4, 5));
        }
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << "Invalid argument for age or tier: " << vec_temp[1] << std::endl;
        return nullptr;
    }

    // Special
    if (vec_temp[1].back() == 'S')
    {
        special = 1;
    }
    else
    {
        special = 0;
    }

    std::string temp_str = vec_temp[2];
    if (temp_str.find("OPN") != std::string::npos)
    {
        division = -1;
        team = -1;
    }
    else
    {
        if (type == GAME)
        {
            team = -1;
        }
        else
        {
            try
            {
                team = std::stoi(vec_temp.back());
            }
            catch (const std::invalid_argument &e)
            {
                std::cerr << "Invalid argument for team: " << vec_temp.back() << std::endl;
                return nullptr;
            }
        }
        // get division, get team number
        try
        {
            division = std::stoi(vec_temp[3]);
        }
        catch (const std::invalid_argument &e)
        {
            std::cerr << "Invalid argument for division: " << vec_temp[3] << std::endl;
            return nullptr;
        }
    }

    GamePractice *temp = new GamePractice(type, league, underOver, age, tier, division, special, team, strIn);
    return temp;
}

GamePractice *Parser::gp_find(std::string &strIn)
{

    // CMSA U12T1 DIV 01 OPN 01
    // CMSA U12T1 DIV 01 PRC 01
    // CMSA U12T1 DIV 01

    // CMSA U09T1 PRC 01

    // DEBUG &&std::cout << "league: " << league << " unit_tier: " << unit_tier
    //                   << " division: " << division << std::endl;
    GamePractice *temp = build_gamePractice(strIn);

    for (auto &gp : gamePractices)
    {
        if (gp == *temp)
        {
            // DEBUG &&std::cout << "Found:\n";
            // DEBUG &&std::cout << "Temp: " << temp.toString() << std::endl;
            // DEBUG &&std::cout << "Found: " << gp.toString() << std::endl;
            return &gp;
        }
    }
    std::cerr << "Couldn't find appropriate GamePractice object\n";
    return nullptr;
}

Slot *Parser::slot_find(GPType type, std::string &strDay, std::string &strTime)
{

    int day = day_convert(strDay);
    int time = time_conversion(strTime);
    if (day == -1 || time == -1)
    {
        std::cerr << "Error: str conversion to Slot in slot find \n";
        return nullptr;
    }
    for (auto &slot : slots)
    {
        if (slot.slotCheck(type, day, time))
        {
            // DEBUG &&std::cout << "Found:\n";
            // DEBUG &&std::cout << "Temp: " << temp.toString() << std::endl;
            // DEBUG &&std::cout << "Found: " << gp.toString() << std::endl;
            return &slot;
        }
    }
    DEBUG &&std::cout << "*** Couldn't find Slot object ***\n";
    return nullptr;
}

int Parser::preferences_factory(std::ifstream &file, std::string next_category, std::streampos &startPosition)
{

    file.seekg(startPosition);
    std::string line;

    while (true)
    {
        if (!std::getline(file, line))
        {
            if (file.eof())
            {
                std::cerr << "End of file reached before " << next_category << " found during compatibility factory\n";
                return int(FLAG_EOF);
            }
            else
            {
                std::cerr << "There was an error reading from the file during not preference factory.\n";
                return FLAG_DNF;
            }
        }
        if (line.find(next_category) != std::string::npos)
        {
            if (DEBUG)
            {
                std::cout << "Preferences: \n";
                for (auto &gp : gamePractices)
                {
                    if (!gp.preferences.empty())
                    {
                        std::cout << gp.toString() << "\n";
                        gp.printPreferences();
                    }
                }
                std::cout << "Finished Creating Preferences.\n ";
            }

            return 0;
        }

        // Remove Windows-style carriage return if present
        if (!line.empty() && line.back() == '\r')
        {
            line.pop_back();
        }

        if (!line.empty())
        {
            // TU, 9:30, CMSA U13T3 DIV 01, 10
            // day, time, GAMEPRACTICE, PREF
            int pref;
            Slot *slot;

            std::vector<std::string> vec_temp;
            split_trim(line, vec_temp);

            GamePractice *f_gp;

            f_gp = gp_find(vec_temp[2]);
            // std::cout << vec_temp[0] << " +++ " << vec_temp[1] << " +++ " << vec_temp[2] << std::endl;
            if (f_gp == nullptr)
            {
                std::cout << "This object doesn't exist so we're ignoring this preference.\n";
            }
            else
            {
                slot = slot_find(f_gp->type, vec_temp[0], vec_temp[1]);
                pref = std::stoi(vec_temp.back());
                // Fix this if it's important to have non-existent slots.
                if (slot != nullptr)
                {
                    // std::cout << "Found Preference for :" << f_gp->toString() << std::endl;
                    f_gp->addPref(slot, pref);
                }
                else
                {
                    std::cout << "This slot doesn't exist so we're ignoring this preference for " << f_gp->toString() << std::endl;
                }
            }
        }
    }

    return 0;
};

int Parser::pair_factory(std::ifstream &file, std::streampos &startPosition)
{
    file.seekg(startPosition);
    std::string line;

    while (true)
    {
        if (!std::getline(file, line))
        {
            if (file.eof())
            {
                std::cerr << "End of file reached before Partial assignments: found during pair factory\n";
                return int(FLAG_EOF);
            }
            else
            {
                std::cerr << "There was an error reading from the file during not pair factory.\n";
                return FLAG_DNF;
            }
        }
        if (line.find("Partial assignments:") != std::string::npos)
        {
            if (DEBUG)
            {
                std::cout << "\nObjects + Pair:\n";
                for (auto &gp : gamePractices)
                {
                    if (!gp.pair.empty())
                    {
                        std::cout << "GAMEPRACTICE OBJ:\t" << gp.toString() << "\n";
                        gp.printPair();
                    }
                }

                std::cout << "Finished Creating Pairs" << std::endl;
            }
            return 0;
        }

        // Remove Windows-style carriage return if present
        if (!line.empty() && line.back() == '\r')
        {
            line.pop_back();
        }

        if (!line.empty())
        {
            std::vector<std::string> vec_temp;
            split_trim(line, vec_temp);

            GamePractice *f_gp;
            GamePractice *s_gp;

            f_gp = gp_find(vec_temp[0]);
            if (f_gp == nullptr)
            {
                std::cout << "couldn't find first in pair. \n";
            }
            else
            {
                s_gp = gp_find(vec_temp[1]);
                if (s_gp == nullptr)
                {
                    std::cout << "Error: Could not find f_gp or S_GP in Pair factory ignoring this.\n";
                }
                else
                {
                    f_gp->addPair(s_gp);
                    s_gp->addPair(f_gp);
                }
            }
            // if (DEBUG)
            // {
            //     f_gp->printNotCompatible();
            //     s_gp->printNotCompatible();
            // }
        }
    }

    return 0;
};

Slot *Parser::find_special_slot()
{
    GPType type = PRACTICE;
    int day = 1;
    int time = 1800;

    for (auto &slot : slots)
    {
        if (slot.slotCheck(type, day, time))
        {
            // DEBUG &&std::cout << "Found:\n";
            // DEBUG &&std::cout << "Temp: " << temp.toString() << std::endl;
            // DEBUG &&std::cout << "Found: " << gp.toString() << std::endl;
            return &slot;
        }
    }
    DEBUG &&std::cout << "*** Couldn't find Slot object for special instance ***\n";
    return nullptr;
}
GamePractice *Parser::create_special_practice(int age, int div)
{
    std::cout << "Attempting to create special practice for age: " << age << std::endl;

    try
    {
        if (leagues.empty())
        {
            std::cerr << "Leagues list is empty.\n";
            return nullptr;
        }
        std::string l = "CMSA";
        std::string *league = find_league_or_add(l);
        int underOver = 0;
        int tier = -1;
        int special = 1;
        int team = -1;
        string idString = std::format("{} {}{} DIV {:02}", l, (underOver ? 'O' : 'U'), age, div);
        GamePractice *newPractice = new GamePractice(PRACTICE, league, underOver, age, tier, div, special, team, idString);
        gamePractices.push_back(*newPractice);
        GamePractice *createdPractice = &gamePractices.back();
        practices.push_back(createdPractice);
        std::cout << "Created new special practice for age: " << age << std::endl;
        return createdPractice;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception occurred while creating special practice for age " << age << ": " << e.what() << std::endl;
        return nullptr;
    }
    catch (...)
    {
        std::cerr << "Unknown error occurred while creating special practice for age " << age << std::endl;
        return nullptr;
    }
}

int Parser::partialAssignment_factory(std::ifstream &file, std::streampos &startPosition)
{
    partialAssignments = Schedule();
    file.seekg(startPosition);
    std::string line;
    bool assign_12 = false;
    bool assign_13 = false;

    for (GamePractice &game : gamePractices)
    {
        if (game.type == GAME)
        {
            std::cout << "Evaluating: " << game.toString() << std::endl;
            if ((game.league) == &leagues[0] && game.underOver == 0 && game.tier == 1)
            {
                if (game.age == 12 || game.age == 13)
                {
                    Slot *special_slot = find_special_slot();
                    if (special_slot == nullptr)
                    {
                        std::cout << "Special case identified for CMSA U12T1/U13T1 but no slot available for practice. \n";
                        return -2;
                    }
                    else if (assign_12 == false && game.age == 12)
                    {
                        GamePractice *temp = create_special_practice(game.age, game.division);
                        if (temp == nullptr)
                        {
                            std::cerr << "Failed to find or create special practice for age 12.\n";
                            return -1;
                        }

                        partialAssignments.AddAssignment(*temp, *special_slot);
                        assign_12 = true;
                    }
                    else if (assign_13 == false && game.age == 13)
                    {
                        GamePractice *temp = create_special_practice(game.age, game.division);
                        if (temp == nullptr)
                        {
                            std::cerr << "Failed to find or create special practice for age 13.\n";
                            return -1;
                        }
                        partialAssignments.AddAssignment(*temp, *special_slot);
                        assign_13 = true;
                    }
                }
            }
        }
    }

    while (true)
    {
        if (!std::getline(file, line))
        {
            if (file.eof())
            {
                std::cout << "Finished Creating Partial assignments\n"
                          << std::endl;
                std::cout << "Size of game practices: " << gamePractices.size() << std::endl;
                std::cout << "Size of Assigned: " << partialAssignments.assignments.size() << std::endl;
                if (DEBUG)
                {
                    std::cout << "Useful debug statement for partial assignments.\n";
                }
                return 0;
            }
            else
            {
                std::cerr << "Error occured during the get line function of partial assignments\n";
                return -1;
            }
        }

        // Remove Windows-style carriage return if present
        if (!line.empty() && line.back() == '\r')
        {
            line.pop_back();
        }

        if (!line.empty())
        {
            std::vector<std::string> vec_temp;
            split_trim(line, vec_temp);

            GamePractice *gp;
            Slot *slot;

            gp = gp_find(vec_temp[0]);
            if (gp == nullptr)
            {
                std::cout << "Partial Assignment of non-existent game detected.\n Invalid Schedule detected.\n ";
                return -2;
            }
            slot = slot_find(gp->type, vec_temp[1], vec_temp[2]);

            // TODO(Kyle): We need to add in logic for Special assignments here.
            if (slot == nullptr)
            {
                std::cout << "Partial Assignment of non-existent slot detected.\n Invalid Schedule detected.\n ";
                return -2;
            }

            // partialAssignments.insert(std::make_pair(gp, slot));
            partialAssignments.AddAssignment(*gp, *slot);

            // if (DEBUG)
            // {
            //     f_gp->printNotCompatible();
            //     s_gp->printNotCompatible();
            // }
        }
    }

    return 0;
}
