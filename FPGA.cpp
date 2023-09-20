// ***********************************************************
// A class for a minimal simulation of an FPGA
// ************************************************************

#include "FPGA.h"
#include "random.h"
#include <stdlib.h>

// **************************** 
// Constructors and Destructors
// ****************************

// The constructor

FPGA::FPGA(int newsize)
{
	SetBlockSize(newsize);
}

// The destructor

FPGA::~FPGA()
{
	SetBlockSize(0);
}

// *********
// Utilities
// *********

// Resize a block.

void FPGA::SetBlockSize(int newsize)
{
	size = newsize;
	states.SetBounds(1,size,1,size);
	states.FillContents(0.0);

	tempstates.SetBounds(1,size,1,size);
	tempstates.FillContents(0.0);
	
	gates.SetBounds(1,size,1,size);
	gates.FillContents(0.0);
	
	Ax.SetBounds(1,size,1,size);
	Ax.FillContents(0.0);
	
	Ay.SetBounds(1,size,1,size);
	Ay.FillContents(0.0);
	
	Bx.SetBounds(1,size,1,size);
	Bx.FillContents(0.0);
	
	By.SetBounds(1,size,1,size);
	By.FillContents(0.0);
}

// *******
// Control
// *******

// Randomize the states or outputs of a circuit.

void FPGA::RandomizeBlockState()
{
	for (int i = 1; i <= size; i++)
		for (int j = 1; j <= size; j++)
        	SetBlockState(i, j, ProbabilisticChoice(0.5));
}

void FPGA::RandomizeBlockState(RandomState &rs)
{
	for (int i = 1; i <= size; i++)
		for (int j = 1; j <= size; j++)
        	SetBlockState(i, j, rs.ProbabilisticChoice(0.5));
}

// Integrate the FPGA one step 

void FPGA::Step()
{
	int A, B;
  // Update the state of all blocks and write into a temporary state
  for (int i = 1; i <= size; i++) {
	for (int j = 1; j <= size; j++) {
		// Get A and B inputs
		A = states[Ax[i][j]][Ay[i][j]];
		B = states[Bx[i][j]][By[i][j]];
		// NAND
		if (gates[i][j] == 0){ 
			if ((A == 1) && (B == 1))
				tempstates[i][j] = 0;
			else
			 	tempstates[i][j] = 1;
		}
		// NOR
		else{
			if ((A == 0) && (B == 0))
				tempstates[i][j] = 1;
			else
			 	tempstates[i][j] = 0;
		}
	}
  }
  // Once calculated, update the main state 
  for (int i = 1; i <= size; i++) {
	for (int j = 1; j <= size; j++) {
		states[i][j] = tempstates[i][j];
	}
  }
}

// ****************
// Input and Output
// ****************

#include <iomanip>

ostream& operator<<(ostream& os, FPGA& c)
{
	// Set the precision
	os << setprecision(32);
	// Write the size
	os << c.size << endl << endl;
	// Write the Gates
	for (int i = 1; i <= c.size; i++) {
		for (int j = 1; j <= c.size; j++)
			os << c.gates[i][j] << " ";
		os << endl;
	}
	os << endl << endl;
	// Write the input A
	for (int i = 1; i <= c.size; i++) {
		for (int j = 1; j <= c.size; j++)
			os << "(" << c.Ax[i][j] << "," << c.Ay[i][j] << ") ";
		os << endl;
	}
	os << endl << endl;
	// Write the input B
	for (int i = 1; i <= c.size; i++) {
		for (int j = 1; j <= c.size; j++)
			os << "(" << c.Bx[i][j] << "," << c.By[i][j] << ") ";
		os << endl;
	}
	os << endl << endl;
	// Return the ostream
	return os;
}

// istream& operator>>(istream& is, CTRNN& c)
// {
// 	// Read the size
// 	int size;
// 	is >> size;
// 	c.SetCircuitSize(size);
// 	// Read the time constants
// 	for (int i = 1; i <= size; i++) {
// 		is >> c.taus[i];
// 		c.Rtaus[i] = 1/c.taus[i];
// 	}
// 	// Read the biases
// 	for (int i = 1; i <= size; i++)
// 		is >> c.biases[i];
// 	// Read the gains
// 	for (int i = 1; i <= size; i++)
// 		is >> c.gains[i];
// 	// Read the weights
// 	for (int i = 1; i <= size; i++)
// 		for (int j = 1; j <= size; j++)
// 			is >> c.weights[i][j];
// 	// Return the istream		
// 	return is;
// }		
		
