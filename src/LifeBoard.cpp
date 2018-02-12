#include "LifeBoard.hpp"

constexpr char LifeBoard::stateSymbols[];

LifeBoard::State& LifeBoard::operator()(int row, int col)
{
	// Take into account periodic boundary conditions.
	row = (row + m_rowCount) % m_rowCount;
	col = (col + m_colCount) % m_colCount;

	// Return 1D index of 1D array corresponding to the 2D index.
	return m_boardData[col + row * m_colCount];
}

const LifeBoard::State& LifeBoard::operator()(int row, int col) const
{
	// Take into account periodic boundary conditions we add extra m_rowCount and m_colCount
	// terms here to take into account the fact that the caller may be indexing with -1.
	row = (row + m_rowCount) % m_rowCount;
	col = (col + m_colCount) % m_colCount;

	// Return 1D index of 1D array corresponding to the 2D index.
	return m_boardData[col + row * m_colCount];
}


LifeBoard::LifeBoard(int rows, int cols, LifeBoard::State state) : 
m_rowCount{rows},
m_colCount{cols},
m_boardData(rows*cols, state)
{}

LifeBoard::LifeBoard(int rows, int cols, std::default_random_engine &generator) : 
m_rowCount{rows},
m_colCount{cols}
{
	//TODO: This can probably be made more efficient.

	// Reserve the correct amount of space in the board data.
	m_boardData.reserve(rows * cols);
	static std::uniform_int_distribution<int> distribution(0,static_cast<int>(LifeBoard::MAXSTATE)-1);
	for(int i = 0; i < rows*cols; ++i)
	{
		m_boardData.push_back(static_cast<LifeBoard::State>(distribution(generator)));
	}
}

void LifeBoard::randomise(std::default_random_engine &generator)
{
	// Create a uniform distribution for the states on the board.
	static std::uniform_int_distribution<int> distribution(0,static_cast<int>(LifeBoard::MAXSTATE)-1);

	for(auto &cell : m_boardData)
	{
		cell = static_cast<LifeBoard::State>(distribution(generator));
	}

}


int LifeBoard::getRows() const
{
	return m_rowCount;
}

int LifeBoard::getCols() const
{
	return m_colCount;
}

int LifeBoard::getSize() const
{
	return m_colCount * m_rowCount;
}

bool LifeBoard::isAlive(int row, int col) const
{
	return (*this)(row,col) == LifeBoard::Alive;
}

int LifeBoard::getAliveNeighbours(int row, int col) const
{
	// This function makes use of isAlive which makes use of operator() overload 
	// which takes into account periodic boundary conditions.
	int sum = 0;

	// Check N.
	sum += (isAlive(row-1,col) ? 1 : 0);
	// Check NE.
	sum += (isAlive(row-1,col+1) ? 1 : 0);
	// Check E.
	sum += (isAlive(row,col+1) ? 1 : 0);
	// Check NS.
	sum += (isAlive(row+1,col+1) ? 1 : 0);
	// Check S.
	sum += (isAlive(row+1,col) ? 1 : 0);
	// Check SW.
	sum += (isAlive(row+1,col-1) ? 1 : 0);
	// Check W.
	sum += (isAlive(row,col-1) ? 1 : 0);
	// Check NW.
	sum += (isAlive(row-1,col-1) ? 1 : 0);

	return sum;

}


LifeBoard::State LifeBoard::nextState(int row, int col) const
{
	// Calculate number of live neighbours, quicker to do it once.
	int liveNeighbours = getAliveNeighbours(row,col);

	// Get the state of the cell currently
	LifeBoard::State currentState = (*this)(row,col);

	// Check whether the state is alive since the update rules are different.
	if(currentState == LifeBoard::Alive)
	{
		// 1. Any live cell with fewer than two live neighbours dies, as if caused by under population.
		if(liveNeighbours < 2) 
		{
			return LifeBoard::Dead;
		}

		// 2. Any live cell with two or three live neighbours lives on to the next generation.
		if(liveNeighbours == 2 || liveNeighbours == 3) 
		{
			return LifeBoard::Alive;
		}

		// 3. Any live cell with more than three live neighbours dies, as if by overpopulation.
		if(liveNeighbours > 3)
		{
			return LifeBoard::Dead;
		}
	}
	if(liveNeighbours == 3)
	// 4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
	{
		return LifeBoard::Alive;
	}
	else
	// Otherwise cell just stays dead.
	{
		return LifeBoard::Dead;
	}
}

void update(LifeBoard &updatedBoard, LifeBoard &currentBoard) 
{
	int maxRows = updatedBoard.getRows();
	int maxCols = updatedBoard.getCols();
	
	for(int row = 0; row < maxRows; ++row)
	{
		for(int col = 0; col < maxCols; ++col)
		{
			updatedBoard(row,col) = currentBoard.nextState(row,col);
		}
	}

}

std::ostream& operator<<(std::ostream& out, const LifeBoard &board)
{
	

	int maxRows = board.getRows();
	int maxCols = board.getCols();

	for(int row = 0; row < maxRows; ++row)
	{
		for(int col = 0; col < maxCols; ++ col)
		{
			out << LifeBoard::stateSymbols[board(row,col)] << ' ';
		}

		out << '\n';
	}

	return out;
}