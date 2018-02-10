#include "LifeBoard.hpp"
#include <random>
#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>
#include <boost/program_options.hpp>

enum RunState
{
	Running,
	Paused,
	Stopped,
	MAXRUNSTATE,
};

int main(int argc, char const *argv[])
{
/*************************************************************************************************************************
************************************************* Preparations **********************************************************
*************************************************************************************************************************/

    // Seed the pseudo random number generator using the system clock.
    unsigned int seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());

    // Create a generator that can be fed to any distribution to produce pseudo random numbers according to that distribution. 
    std::default_random_engine generator(seed);

    // Input parameters.
    int rowCount;
    int colCount;
    int outputFrequency;

    // Set up optional command line argument.
    boost::program_options::options_description desc("Options for Game of Life program");

    // Add all optional command line arguments.
    desc.add_options()
        
        ("column-count,c", boost::program_options::value<int>(&rowCount)->default_value(50), "The number of rows in the board.")
        ("row-count,r", boost::program_options::value<int>(&colCount)->default_value(50), "The number of columns in the board.")
        ("output-frequency,f", boost::program_options::value<int>(&outputFrequency)->default_value(100), "The pause time between outputting the updated board")
        ("help,h", "produce help message");

    // Make arguments available to program.
    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc,argv,desc), vm);
    boost::program_options::notify(vm);

    // If the user asks for help display it then exit.
    if(vm.count("help"))
    {
        std::cout << desc << '\n';
        return 1;
    }


    // Create a board that represents the current state of the system.
	LifeBoard boardCurrent(rowCount, colCount, generator);

	// Create a board that represents the updated state of the system making sure it is initially the same as the current board.
	LifeBoard boardUpdated = boardCurrent;

	// Variable to hold the state of the 
	RunState runState = Running;

/*************************************************************************************************************************
************************************************* Main Loop *************************************************************
*************************************************************************************************************************/

	
	while(true)
	{
		// Update the board.
		update(boardUpdated, boardCurrent);
	
		// Print the updated board.
		std::cout << boardUpdated;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));



		// Move the cursor back to the top of the board.
		for(int row = 0; row < boardCurrent.getRows(); ++row)
		{
			std::cout << "\e[A";
			std::cout << "\r";
		}

		// Swap the boards so no unnecessary copying takes place.
		std::swap(boardUpdated, boardCurrent);

		// Check if the system has reached a steady state and if it has break the loop.

	}

/*************************************************************************************************************************
************************************************* Clean Up *************************************************************
*************************************************************************************************************************/

	return 0;
}
