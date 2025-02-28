#include <vector>
#include "types.hpp"
#include "environment.hpp"
using namespace std;

vector<std::string> leagues = {"CMSA", "CUSA", "CSSC"};

class GamePractice;
class Slot;

vector<Slot> slots;
vector<Slot *> gameslots;
vector<Slot *> practiceslots;

vector<GamePractice> gamePractices;
vector<GamePractice *> games;
vector<GamePractice *> practices;
Schedule partialAssignments;

Penalties penalties;
Weights weights;


// TODO(Dan & Kyle): gotta address this change
// map<GamePractice *, Slot *> partialAssignments;
namespace Environment
{
    size_t POP_INIT = 50;
    size_t POP_CAP = 100;
    float MUT_PROB = 0.7f;
    float MUT_AMOUNT = 0.5f;
    float CROSS_PROP = 0.2f;
    float DEL_PROP = 0.5f;
    long MAX_RUNTIME = 20;
    int MAX_OTREE_TIME = 10;

    void LoadEnvironmentConfig()
    {
        string filename = "./environment.txt";
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Error: Unable to open file " + filename);
        }

        std::vector<std::string> lines;
        std::string line;

        // Read all lines from the file
        while (std::getline(file, line)) {
            // Handle Windows-style line endings by stripping '\r'
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }
            lines.push_back(line);
        }

        if (lines.size() != 8) { // Ensure there are exactly 8 values
            throw std::runtime_error("Error: Config file must contain exactly 8 lines.");
        }

        // Parse values and assign them to the respective variables
        try {
            Environment::POP_INIT = std::stoul(lines[0]);
            Environment::POP_CAP = std::stoul(lines[1]);
            Environment::MUT_PROB = std::stof(lines[2]);
            Environment::MUT_AMOUNT = std::stof(lines[3]);
            Environment::CROSS_PROP = std::stof(lines[4]);
            Environment::DEL_PROP = std::stof(lines[5]);
            Environment::MAX_RUNTIME = std::stol(lines[6]);
            Environment::MAX_OTREE_TIME = std::stoi(lines[7]);
        } catch (const std::exception& e) {
            throw std::runtime_error("Error: Invalid data in config file. " + std::string(e.what()));
        }
    }
}