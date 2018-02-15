#ifndef LifeBoard_hpp
#define LifeBoard_hpp

#include <vector> // For holding the data in the array.
#include <random> // For generating random numbers.
#include <iostream> // For outputting board.

/**
 * \file
 * \brief Class to model a 2D lattice of cells that can be dead or alive.
 */
class LifeBoard
{
public:
	/** 
	 * \enum State
	 * \brief Enumeration type to hold the state of the cell, dead or alive.
	 */
	enum State
	{
		Dead,
		Alive,
		MAXSTATE,
	};

	/// Look-up table for alive/dead cells symbols for printing.
	static constexpr char stateSymbols[MAXSTATE] = {' ','o'};

private:
	/// Member variable that holds number of rows in lattice.
	int m_rowCount;

	/// Member variable that holds number of columns in lattice.
	int m_colCount;

	/// Member variable that holds the actual data in the lattice.
	std::vector<State> m_boardData;

public:
	/**
	 *\brief operator overload for getting the state at a site.
	 *
	 * This method is implemented since the states are stored internally as a 1D vector, hence 
	 * they need to be indexed in a special way in order to get the site that would correspond to 
	 * the (i,j) site in matrix notation. This function allows the caller to treat the lattice as a 
	 * 2D matrix without having to worry about the internal implementation.
	 *
	 *\param row row index of site.
	 *\param col column index of site.
	 *\return reference to state stored at site so called can use it or set it.
	 */
	LifeBoard::State& operator()(int row, int col);

	/** 
	 *\brief constant version of non-constant counterpart for use with constant LifeBoard object.
	 *
	 * See non-constant version for description.
	 *
	 *\param row row index of site.
	 *\param col column index of site.
	 *\return constant reference to state stored at site so called can use it only.
	 */
	const LifeBoard::State& operator()(int row, int col) const;

	/**
	 *\brief Constructor that initializes all cells to the state that is its arguments.
	 *\param rows number of rows on the board.
	 *\param cols number of columns on the board.
	 *\param state State instance to initialise all cells to will default to alive.
	 */
	LifeBoard(int rows, int cols, LifeBoard::State state = LifeBoard::Alive);

	/** 
	 *\brief Constructor that randomises lattice to an even mix of states.
	 *\param rows number of rows on the board.
	 *\param cols number of columns on the board.
	 *\param generator std::default_random_engine reference for generating random numbers.
	 */
	LifeBoard(int rows, int cols, std::default_random_engine &generator);

	/**
	 *\brief Randomises the cells in the board with equal probability of being dead or alive.
	 *\param std::deafult_random_engine reference for random number generation.
	 */
	void randomise(std::default_random_engine &generator);

	/**
	 *\brief Getter for the number of rows.
	 *\return Integer value representing the number of rows.
	 */
	int getRows() const;

	/**
	 *\brief Getter for number of columns.
	 *\return Integer value representing the number of columns.
	 */
	int getCols() const;

	/**
	 *\brief Getter for size of lattice #rows * #columns.
	 *\return Integer value representing the size of the lattice.
	 */
	int getSize() const;

	/**
	 *\brief Calculates whether given cell is alive or dead.
	 *\param row row of cell in question.
	 *\param col column of cell in question.
	 *\return boolean value representing result of query.
	 */
	bool isAlive(int row, int col) const;

	/**
	 *\brief Calculates the number of alive nearest neighbours.
	 *\param row row of cell of interest.
	 *\param col column of cell of interest
	 *\return Integer value representing the number of alive nearest neighbours.
	 *
	 * We assume the Moore neighbourhood so there are 8 nearest neighbours 
	 * at the compass positions N, NE, E, SE, S, SW, W, NW. 
	 */
	int getAliveNeighbours(int row, int col) const;

	/**
	 *\brief Calculates whether the cell will be dead or alive on the next iteration.
	 *\param row row of cell of interest.
	 *\param col column of cell of interest.
	 *\return State representing the future state of the cell.
	 *
	 * This function DOES NOT actually update the cell it just tells the caller the state of the cell 
	 * on the next iteration. The rules for GOL are taken to be as follows:
	 * 1. Any live cell with fewer than two live neighbours dies, as if caused by under population.
	 * 2. Any live cell with two or three live neighbours lives on to the next generation.
     * 3. Any live cell with more than three live neighbours dies, as if by overpopulation.
	 * 4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
	 *
	 */
	LifeBoard::State nextState(int row, int col) const;

	/**
	 *\brief Updates the board based on the rules for the GOL.
	 *\param updatedBoard LifeBoard object that will be the updated board.
	 *\param currentBoard LifeBoard object that is the board the update is based on.
	 */
	friend void update(LifeBoard &updatedBoard, LifeBoard &currentBoard);

	/**
	 *\brief streams the board to an output stream in a nicely formatted way
	 *\param out std::ostream reference that is being streamed to 
	 *\param board LifeBoard reference to be printed
	 *\return std::ostream reference to output can be chained.
	 */
	 friend std::ostream& operator<<(std::ostream& out, const LifeBoard &board);

	 



};

#endif /* LifeBoard_hpp */