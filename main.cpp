#include "TSearch.h"
#include "FPGA.h"
#include "random.h"

// #define PRINTOFILE

// Task params
const double DUR = 100; 

// EA params
const int POPSIZE = 96;
const int GENS = 100;
const double MUTVAR = 0.05;			
const double CROSSPROB = 0.0;
const double EXPECTED = 1.1;
const double ELITISM = 0.02;

// FPGA params
const int N = 2; 

int	VectSize = 5*N*N; // 2 bits for input A, 2 bits for input B, and 1 bit for Gate

// ================================================
// A. FUNCTIONS FOR EVOLVING A SUCCESFUL CIRCUIT
// ================================================

// ------------------------------------
// Fitness function
// ------------------------------------
double FitnessFunction(TVector<double> &genotype, RandomState &rs)
{
	// Create the board
	FPGA board(N);

	return 0.0;
}

// ------------------------------------
// Behavior
// ------------------------------------
double Behavior()
{
	// Create the board
	FPGA board(N);

	TVector<int> g;
	g.SetBounds(1,VectSize);
	g[0]=0; g[1]=1; g[2]=1; g[3]=1; g[4]=0;
	g[5]=1; g[6]=0; g[7]=1; g[8]=1; g[9]=1;
	g[10]=0; g[11]=0; g[12]=0; g[13]=1; g[14]=1;
	g[15]=1; g[16]=0; g[17]=0; g[18]=0; g[19]=0;
	cout << g << endl;

	// Set gates and inputs from genotype to board
	int k = 0;
	for (int i = 1; i <= N; i++){
		for (int j = 1; j <= N; j++){
			// Input A (does not check for in-bounds)
			if ((g[k] == 0) && (g[k+1] == 0)){  // North
				board.Ax[i][j] = i;
				board.Ay[i][j] = j-1;
			}
			else{
				if ((g[k] == 0) && (g[k+1] == 1)){  // East
					board.Ax[i][j] = i+1;
					board.Ay[i][j] = j;
				}
				else{
					if ((g[k] == 1) && (g[k+1] == 0)){	// West
						board.Ax[i][j] = i-1;
						board.Ay[i][j] = j;
					}
					else{		// South
							board.Ax[i][j] = i;
							board.Ay[i][j] = j+1;						
					}
				}
			}
			k+=2;
			// Input B (does not check for in-bounds)
			if ((g[k] == 0) && (g[k+1] == 0)){  // North
				board.Bx[i][j] = i;
				board.By[i][j] = j-1;
			}
			else{
				if ((g[k] == 0) && (g[k+1] == 1)){  // East
					board.Bx[i][j] = i+1;
					board.By[i][j] = j;
				}
				else{
					if ((g[k] == 1) && (g[k+1] == 0)){	// West
						board.Bx[i][j] = i-1;
						board.By[i][j] = j;
					}
					else{		// South
							board.Bx[i][j] = i;
							board.By[i][j] = j+1;						
					}
				}
			}
			k+=2;
			// Gate
			board.gates[i][j] = g[k];
			k++;
		}
	}

	// Initialize state
	board.ZeroBlockState();

	cout << board << endl;


	return 0.0;
}


// ------------------------------------
// Display functions
// ------------------------------------
void EvolutionaryRunDisplay(int Generation, double BestPerf, double AvgPerf, double PerfVar)
{
	cout << Generation << " " << BestPerf << " " << AvgPerf << " " << PerfVar << endl;
}

void ResultsDisplay(TSearch &s)
{
	TVector<double> bestVector;
	ofstream BestIndividualFile;

	// Save the genotype of the best individual
	bestVector = s.BestIndividual();
	BestIndividualFile.open("best.gen.dat");
	BestIndividualFile << bestVector << endl;
	BestIndividualFile.close();
}


// ------------------------------------
// The main program
// ------------------------------------
int main (int argc, const char* argv[]) 
{
	// long randomseed = static_cast<long>(time(NULL));
	// if (argc == 2)
	// 	randomseed += atoi(argv[1]);

	// TSearch s(VectSize);
	
	// #ifdef PRINTOFILE
	// ofstream file;
	// file.open("evol.dat");
	// cout.rdbuf(file.rdbuf());
	// // save the seed to a file
	// ofstream seedfile;
	// seedfile.open ("seed.dat");
	// seedfile << randomseed << endl;
	// seedfile.close();
	// #endif
	
	// // Configure the search
	// s.SetRandomSeed(randomseed);
	// s.SetSearchResultsDisplayFunction(ResultsDisplay);
	// s.SetPopulationStatisticsDisplayFunction(EvolutionaryRunDisplay);
	// s.SetSelectionMode(RANK_BASED);
	// s.SetReproductionMode(GENETIC_ALGORITHM);
	// s.SetPopulationSize(POPSIZE);
	// s.SetMaxGenerations(GENS);
	// s.SetCrossoverProbability(CROSSPROB);
	// s.SetCrossoverMode(UNIFORM);
	// s.SetMutationVariance(MUTVAR);
	// s.SetMaxExpectedOffspring(EXPECTED);
	// s.SetElitistFraction(ELITISM);
	// s.SetSearchConstraint(1);
	
	// s.SetEvaluationFunction(FitnessFunction); 
	// s.ExecuteSearch();
	
	Behavior();
	// return 0;
}
