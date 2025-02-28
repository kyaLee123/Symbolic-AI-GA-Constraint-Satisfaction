#include <iostream>
#include <string>
#include <vector>
#include <random>
#include "types.hpp"
#include "process.hpp"
#include "environment.hpp"
#include "Parser.hpp"

using namespace std; // Can access standard library objects without std::
// int main()
int main(int argc, char *argv[]) // Let's just suppose this is our search instance
{
    if (argc != 10)
    {
        std::cerr << "Incorrect argc length " << argc << " != 10.\n Please enter the correct arguments and try again.\n";
        return -1;
    }

    std::string fd = argv[1];
    weights.wMinFilled = std::stoi(argv[2]);
    weights.wPref = std::stoi(argv[3]);
    weights.wPair = std::stoi(argv[4]);
    weights.wSecDiff = std::stoi(argv[5]);

    penalties.gamemin = std::stoi(argv[6]);
    penalties.practicemin = std::stoi(argv[7]);
    penalties.notpaired = std::stoi(argv[8]);
    penalties.pen_section = std::stoi(argv[9]);

    // penalties.gamemin = 1;
    // penalties.notpaired = 1;
    // penalties.pen_section = 1;
    // penalties.practicemin = 1;

    // games = vector<GamePractice *>();
    // practices = vector<GamePractice *>();
    // for (GamePractice gp : gamePractices)
    // {
    //     if (gp.type == GAME)
    //         games.push_back(&gp);
    //     else if (gp.type == PRACTICE)
    //         practices.push_back(&gp);
    // }

    // penalties.gamemin = 1;
    // penalties.notpaired = 1;
    // penalties.pen_section = 1;
    // penalties.practicemin = 1;

    // weights.wMinFilled = 2;
    // weights.wPair = 1;
    // weights.wPref = 5;
    // weights.wSecDiff = 6;

    // Define the test file name
    // std::string test_file = "test.txt";

    // Create a Parser object
    Parser parser(fd);

    // Parse the file
    int result = parser.parse();
    cout<<"made it past result!"<<endl;

    // Check the result and print the parsed data
    if (result == 0)
    {
        std::cout << "Parsing successful in main!" << std::endl;
    }
    else if (result == -1)
    {
        std::cerr << "Parsing failed with error code: " << result << std::endl;
        exit(EXIT_FAILURE);
    }
    else
    {
        std::cout << "Invalid schedule detected in the parsing.\n";
        return 0;
    }

    std::cout << "printing game practices" << endl;
    std::cout << gamePractices.size() << endl;
    for (GamePractice gp : gamePractices)
    {
        gp.printSelf();
    }
    std::cout << "running process" << std::endl;
    cout<<"partial assignments size: "<<endl;
    cout<<partialAssignments.getSize()<<endl;

    cout<<"partial assignments: "<<endl;
    for (pair<GamePractice*, Slot*> assign : partialAssignments.assignments){
        cout<<"GAME: "<<assign.first->toString()<<" PRAC: "<<assign.second->timeStart<<endl;
    }
   
    Environment::LoadEnvironmentConfig(); // Loads the config from a file so it can be changed without recompiling

    if (!partialAssignments.IsValid())
    {
        cerr << "PARTIAL ASSIGNMENT IS INVALID" << endl;
        return EXIT_FAILURE;
    }
    Process searchProcess = Process();
    searchProcess.Run();
    searchProcess.SaveResult(fd);

    return EXIT_SUCCESS;
}