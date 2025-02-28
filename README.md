# About the project
This project was motivated by finding a way to optimize the time it takes to solve schedueling problems. Instead of using the typical solution to this -- 
some form of an AND tree, I decided to go with the Genetic Algorithm Route, in hopes that this would reduce the ammount of search time for this problem,
while still achieving optimal results. In this README, I will go over each aspect of the Genetic Algorithm's functionality.

For further information on the desicions and specifics of this project, please [View my proposal paper](https://github.com/kyaLee123/Symbolic-AI-GA-Constraint-Satisfaction/blob/main/ProposalGA.pdf)


## 1. The population and genomes
The genomes, in this project, were tuples of (games/practices) assigned to (timeslots). We assign Games/Practices to a timeslot. An individual 
is a complete schedule, of which passes all hard consrtaints (every game/practice is assignd to a timeslot). The population is a set of
multiple complete and viable schedules.
## 2. Generating an initial population
I decided that any member of the GA's population had to be a valid schedule, that passes all of the hard constraints. Considering this, I decided to
implement an OrTree in the generation of this population. The OrTree had a couple of additional layers to it to make the tree run faster -- 
it assigned teams with the most strict constraints first, and will abandon a branch after 10 unsuccesful matches in a row. The Otree, as a result, runs 
fairly fast, and with a population of 50, it typically takes less than 1 second to generate a population.
## 3. The Process
The GA runs once we have an initial population. It runs in a process of iterations, where each iteration is either a **crossover**, **mutation**, or **deletion**.
When delete when the population reaches a certain **cap** variable. Otherwise, we crossover x% of the time, and mutate y%, where x+y = 100. After preforming crossover or mutation, we make the newly generated schedule a new member of our population.
## 4. Preforming Crossover
When crossover is selected as our iteration function, we select 2 random population members (aka the "parents for our crossover) from the best 30% of the population, to crossover. We allow the generated child to only be a valid schedule. 
To implement this rule, we again must use an OrTree. The rules for this or tree are the following: for each game/practice in the child schedule, try to assign it to either the associated timeslot from parent A or parent B. In the situation that both parent's timeslots cause a conflict, select a random timeslot for this game/practice. From this, we will achieve a resulting valid child schedule, and we add it to the population.
## 5. Preforming Mutation
When mutation is selected as our iteration function, we select a random member of our population to undergo a mutation. In this project, we implemented multi-point mutation, wherein we changed the timeslots associated
with multiple of the games/practices in our original selected schedule. These were selected randomly and again, assigned by an optimized ortree.
## 6. Deletion (AKA Plague)
When our population reached some **cap** variable, we deleted some x% (typically this number is about 50) of our population. This allowed us to prioritize only the greatest members of our population.

## 7. Conclusions on the project
In this project, we were able to succesfully implement a genetic algorithm to solve a complex scheduleing problem. 

# How to Build
Ensure CMake is installed
Run the following commands in order:
chmod +x ./build.sh
./build.sh
IF SHELL SCRIPTS AREN'T ENABLED:
cmake -B ./build/ .
make -C ./build/

# How to run
./build/AIProject filename params

# Results
Our output from running the program for 10 minutes are in results.txt.

# LLM Usage
We used tools such as ChatGPT and copilot to assist with programming and individual functions, but most of the code was designed and written by us.



# More Information
## Environment
We store the environment variables in the file environment.txt. It is formatted as follows:

Initial Population
Max Population (Delete threshold)
Mutation Probability: The chance of mutation being selected over crossover
Mutation Proportion: The proportion of changes made to a schedule in mutation compared to the whole schedule
Crossover Proportion: Upper proportion of the population that can be selected for crossover
Delete Proportion: Proportion of the population deleted when delete is called
Max Runtime: Time the program can run for in seconds
Max Otree Time: Max time otree can take before giving up in seconds

This file allows us to change the environment variables without building the program again. It is currently set to 10 minutes,
which we found gave a decent result. For running smaller files we recommend changing Max Runtime to 1-10 seconds.

## Tests
We have two shell scripts for running the demo tests, runhardtests.sh and runsofttests.sh. These run each test with a pause to view the output in between.
We also used test.txt and some others to ensure our constraints and eval work.

## Data visualization
We have included a method for graphing the results in ./results/. The program stores the results over each generation, and outputs them as follows:
    Input: infilename.txt     Output: infilename_output.csv
The Jupyter notebook loads the files for the LargeInput files and graphs them, though you can modify it to graph results from other input files.
The graphs show the best, 90th percentile, median, and worst evals in the population.



Special Thanks to Jorg for failing Gui and Dan last year on the final, we wouldn't be here without him!
