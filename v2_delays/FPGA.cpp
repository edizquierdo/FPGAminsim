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
	states.FillContents(0);

	maxdelay = 3;
	history.SetBounds(1,size*size,1,maxdelay);
	history.FillContents(0);

	// delays.SetBounds(1,size,1,size);
	// delays.FillContents(1);
	
	gates.SetBounds(1,size,1,size);
	gates.FillContents(0);
	
	Ax.SetBounds(1,size,1,size);
	Ax.FillContents(0);
	
	Ay.SetBounds(1,size,1,size);
	Ay.FillContents(0);
	
	Bx.SetBounds(1,size,1,size);
	Bx.FillContents(0);
	
	By.SetBounds(1,size,1,size);
	By.FillContents(0);
}

// *******
// Control
// *******

// Initialize the states or outputs of a circuit.
void FPGA::ZeroBlockState()
{
	for (int i = 1; i <= size; i++){
		for (int j = 1; j <= size; j++){
        	SetBlockState(i, j, 0);
		}
	}
}

void FPGA::OneBlockState()
{
	for (int i = 1; i <= size; i++){
		for (int j = 1; j <= size; j++){
        	SetBlockState(i, j, 1);
		}
	}
}

void FPGA::RandomizeBlockState()
{
	for (int i = 1; i <= size; i++){
		for (int j = 1; j <= size; j++){
			//cout << i << " " << j << " " << ProbabilisticChoice(0.5) << endl;
        	SetBlockState(i, j, ProbabilisticChoice(0.5));
		}
	}
}

void FPGA::RandomizeBlockState(RandomState &rs)
{
	for (int i = 1; i <= size; i++){
		for (int j = 1; j <= size; j++){
        	SetBlockState(i, j, rs.ProbabilisticChoice(0.5));
		}
	}
}

void FPGA::UpdateHistory()
{
	// First, push each item back one time step
	for (int t = 1; t < maxdelay; t++){
		for (int i = 1; i <= size; i++) {
			for (int j = 1; j <= size; j++) {
				history[i+size*(j-1)][t+1] = history[i+size*(j-1)][t];
			}
		}
	}
	// Make the most recent one the current one
	for (int i = 1; i <= size; i++) {
		for (int j = 1; j <= size; j++) {
			history[i+size*(j-1)][1] = states[i][j];
		}
	}
}

// Integrate the FPGA one step 

void FPGA::Step()
{
	int A, B, x, y, delay;
  // Update the state of all blocks and write into a temporary state
  for (int i = 1; i <= size; i++) {
	for (int j = 1; j <= size; j++) {
		// Get A and B inputs
		x = Ax[i][j];
		y = Ay[i][j];
		if ((x>0) && (x<=size) && (y>0) && (y<=size)){
			// Make sure they are referring to an actual block, otherwise input set to 0
			// Set it according to their respective delay
			if (gates[x][y] == 0){ 
				delay = 1; //No delay //2;
			}
			else{
				delay = 1; //No delay //3;
			}
			A = history[x+size*(y-1)][delay];
		}
		else{
			A = 0;
		}
		x = Bx[i][j];
		y = By[i][j];		
		if ((x>0) && (x<=size) && (y>0) && (y<=size)){
			// Make sure they are referring to an actual block, otherwise input set to 0
			if (gates[x][y] == 0){ 
				delay = 1; //No delay //2;
			}
			else{
				delay = 1; //No delay //3;
			}
			B = history[x+size*(y-1)][delay];

		}
		else{
			B = 0;
		}
		// NAND
		if (gates[i][j] == 0){ 
			if ((A == 1) && (B == 1))
				states[i][j] = 0;
			else
			 	states[i][j] = 1;
		}
		// NOR
		else{
			if ((A == 0) && (B == 0))
				states[i][j] = 1;
			else
			 	states[i][j] = 0;
		}
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
		
