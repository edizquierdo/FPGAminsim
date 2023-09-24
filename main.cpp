#include "BSearch.h"
#include "FPGA.h"
#include "random.h"

#define PRINTOFILE

// Task params
const double DUR = 10; 

// EA params
const int POPSIZE = 32; //96;
const int GENS = 100; //100;
const double MUTVAR = 0.05;			
const double CROSSPROB = 0.0;
const double EXPECTED = 1.1;
const double ELITISM = 0.02;

// FPGA params
const int N = 3; 

int	VectSize = 5*N*N; // 2 bits for input A, 2 bits for input B, and 1 bit for Gate

// ================================================
// A. FUNCTIONS FOR EVOLVING A SUCCESFUL CIRCUIT
// ================================================

// ------------------------------------
// Fitness function
// ------------------------------------
double FitnessFunction(TVector<int> &g, RandomState &rs)
{
	//cout << "\n" << g << endl;

	// Create the board
	FPGA board(N);

	// Change the genotype to binary phenotype
	// TVector<int> g;
	// g.SetBounds(1,VectSize);
	// for (int i = 1; i <= VectSize; i++){
	// 	if (genotype[i] >= 0){
	// 		g[i] = 1;
	// 	}
	// 	else{
	// 		g[i] = 0;
	// 	}
	// }

	// Set gates and inputs from genotype to board
	int k = 1;
	for (int i = 1; i <= N; i++){
		for (int j = 1; j <= N; j++){
			// Input A (does not check for in-bounds)
			if ((g[k] == 0) && (g[k+1] == 0)){  // North
				board.Ax[i][j] = i-1;
				board.Ay[i][j] = j;
			}
			else{
				if ((g[k] == 0) && (g[k+1] == 1)){  // East
					board.Ax[i][j] = i;
					board.Ay[i][j] = j+1;
				}
				else{
					if ((g[k] == 1) && (g[k+1] == 0)){	// West
						board.Ax[i][j] = i;
						board.Ay[i][j] = j-1;
					}
					else{		// South
							board.Ax[i][j] = i+1;
							board.Ay[i][j] = j;						
					}
				}
			}
			k+=2;
			// Input B (does not check for in-bounds)
			if ((g[k] == 0) && (g[k+1] == 0)){  // North
				board.Bx[i][j] = i-1;
				board.By[i][j] = j;
			}
			else{
				if ((g[k] == 0) && (g[k+1] == 1)){  // East
					board.Bx[i][j] = i;
					board.By[i][j] = j+1;
				}
				else{
					if ((g[k] == 1) && (g[k+1] == 0)){	// West
						board.Bx[i][j] = i;
						board.By[i][j] = j-1;
					}
					else{		// South
							board.Bx[i][j] = i+1;
							board.By[i][j] = j;						
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

	//cout << board.gates << endl;
	
	// Count the number of times blocks change
	double total = 0.0;
	TMatrix<int> previousboardstate;
	previousboardstate.SetBounds(1,N,1,N);
	previousboardstate.FillContents(0.0);
	// Save latest state
	for (int i = 1; i <= N; i++){
		for (int j = 1; j <= N; j++){
			previousboardstate[i][j] = board.states[i][j];
		}
	}
	// Simulate for DUR steps
	for (int t = 1; t <= DUR; t++){
		board.Step();
		double portion = 0.0;
		// Analyze difference and count
		for (int i = 1; i <= N; i++){
			for (int j = 1; j <= N; j++){
				if (board.states[i][j] != previousboardstate[i][j]){
					portion += 1.0;
				}
			}
		}
		total += portion/(N*N);
		// Save latest state
		for (int i = 1; i <= N; i++){
			for (int j = 1; j <= N; j++){
				previousboardstate[i][j] = board.states[i][j];
			}
		}
	}
	//cout << total/DUR << endl;
	return total/DUR;
}

// ------------------------------------
// 2N Designed Behavior
// ------------------------------------
double DesignedBehavior()
{
	// Create the board
	FPGA board(N);

	TVector<int> g;
	g.SetBounds(1,VectSize);

	g[1]=0; g[2]=1; g[3]=1; g[4]=1; g[5]=1;
	g[6]=1; g[7]=0; g[8]=1; g[9]=1; g[10]=0;
	g[11]=0; g[12]=0; g[13]=0; g[14]=1; g[15]=0;
	g[16]=1; g[17]=0; g[18]=0; g[19]=0; g[20]=1;
	cout << g << endl;

	// Set gates and inputs from genotype to board
	int k = 1;
	for (int i = 1; i <= N; i++){
		for (int j = 1; j <= N; j++){
			// Input A (does not check for in-bounds)
			if ((g[k] == 0) && (g[k+1] == 0)){  // North
				board.Ax[i][j] = i-1;
				board.Ay[i][j] = j;
			}
			else{
				if ((g[k] == 0) && (g[k+1] == 1)){  // East
					board.Ax[i][j] = i;
					board.Ay[i][j] = j+1;
				}
				else{
					if ((g[k] == 1) && (g[k+1] == 0)){	// West
						board.Ax[i][j] = i;
						board.Ay[i][j] = j-1;
					}
					else{		// South
							board.Ax[i][j] = i+1;
							board.Ay[i][j] = j;						
					}
				}
			}
			k+=2;
			// Input B (does not check for in-bounds)
			if ((g[k] == 0) && (g[k+1] == 0)){  // North
				board.Bx[i][j] = i-1;
				board.By[i][j] = j;
			}
			else{
				if ((g[k] == 0) && (g[k+1] == 1)){  // East
					board.Bx[i][j] = i;
					board.By[i][j] = j+1;
				}
				else{
					if ((g[k] == 1) && (g[k+1] == 0)){	// West
						board.Bx[i][j] = i;
						board.By[i][j] = j-1;
					}
					else{		// South
							board.Bx[i][j] = i+1;
							board.By[i][j] = j;						
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
	//board.RandomizeBlockState();

	cout << board << endl;

	// Simulate X times
	cout << board.states << endl;
	// Write to file
	ofstream fout("states.dat");
	for (int i = 1; i <= N; i++)
	{
		for (int j = 1; j <= N; j++)
		{
			fout << board.states[i][j] << " ";
		}
	}
	fout << endl;
	for (int i = 0; i < 8; i++){
		board.Step();
		cout << ">" << i+1 << "\n" << board.states << endl;
		for (int i = 1; i <= N; i++)
		{
			for (int j = 1; j <= N; j++)
			{
				fout << board.states[i][j] << " ";
			}
		}
		fout << endl;
	}
	fout.close();


	return 0.0;
}

// ------------------------------------
// N Behavior
// ------------------------------------
double RandomBehavior()
{
	// Create the board
	FPGA board(N);

	TVector<int> g;
	g.SetBounds(1,VectSize);
	for (int i = 0; i < VectSize; i++){
		g[i] = ProbabilisticChoice(0.5);
	}
	cout << g << endl;

	// Set gates and inputs from genotype to board
	int k = 1;
	for (int i = 1; i <= N; i++){
		for (int j = 1; j <= N; j++){
			// Input A (does not check for in-bounds)
			if ((g[k] == 0) && (g[k+1] == 0)){  // North
				board.Ax[i][j] = i-1;
				board.Ay[i][j] = j;
			}
			else{
				if ((g[k] == 0) && (g[k+1] == 1)){  // East
					board.Ax[i][j] = i;
					board.Ay[i][j] = j+1;
				}
				else{
					if ((g[k] == 1) && (g[k+1] == 0)){	// West
						board.Ax[i][j] = i;
						board.Ay[i][j] = j-1;
					}
					else{		// South
							board.Ax[i][j] = i+1;
							board.Ay[i][j] = j;						
					}
				}
			}
			k+=2;
			// Input B (does not check for in-bounds)
			if ((g[k] == 0) && (g[k+1] == 0)){  // North
				board.Bx[i][j] = i-1;
				board.By[i][j] = j;
			}
			else{
				if ((g[k] == 0) && (g[k+1] == 1)){  // East
					board.Bx[i][j] = i;
					board.By[i][j] = j+1;
				}
				else{
					if ((g[k] == 1) && (g[k+1] == 0)){	// West
						board.Bx[i][j] = i;
						board.By[i][j] = j-1;
					}
					else{		// South
							board.Bx[i][j] = i+1;
							board.By[i][j] = j;						
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
	//board.ZeroBlockState();
	board.RandomizeBlockState();

	cout << board << endl;

	// Simulate X times
	cout << board.states << endl;
	// Write to file
	ofstream fout("states.dat");
	for (int i = 1; i <= N; i++)
	{
		for (int j = 1; j <= N; j++)
		{
			fout << board.states[i][j] << " ";
		}
	}
	fout << endl;
	for (int i = 0; i < 8; i++){
		board.Step();
		cout << ">" << i+1 << "\n" << board.states << endl;
		for (int i = 1; i <= N; i++)
		{
			for (int j = 1; j <= N; j++)
			{
				fout << board.states[i][j] << " ";
			}
		}
		fout << endl;
	}
	fout.close();

	return 0.0;
}

// ------------------------------------
// Fitness function
// ------------------------------------
double RecordBehavior(TVector<int> &g)
{
	// Create the board
	FPGA board(N);

	// Set gates and inputs from genotype to board
	int k = 1;
	for (int i = 1; i <= N; i++){
		for (int j = 1; j <= N; j++){
			// Input A (does not check for in-bounds)
			if ((g[k] == 0) && (g[k+1] == 0)){  // North
				board.Ax[i][j] = i-1;
				board.Ay[i][j] = j;
			}
			else{
				if ((g[k] == 0) && (g[k+1] == 1)){  // East
					board.Ax[i][j] = i;
					board.Ay[i][j] = j+1;
				}
				else{
					if ((g[k] == 1) && (g[k+1] == 0)){	// West
						board.Ax[i][j] = i;
						board.Ay[i][j] = j-1;
					}
					else{		// South
							board.Ax[i][j] = i+1;
							board.Ay[i][j] = j;						
					}
				}
			}
			k+=2;
			// Input B (does not check for in-bounds)
			if ((g[k] == 0) && (g[k+1] == 0)){  // North
				board.Bx[i][j] = i-1;
				board.By[i][j] = j;
			}
			else{
				if ((g[k] == 0) && (g[k+1] == 1)){  // East
					board.Bx[i][j] = i;
					board.By[i][j] = j+1;
				}
				else{
					if ((g[k] == 1) && (g[k+1] == 0)){	// West
						board.Bx[i][j] = i;
						board.By[i][j] = j-1;
					}
					else{		// South
							board.Bx[i][j] = i+1;
							board.By[i][j] = j;						
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

	cout << board.gates << endl;
	
	// Count the number of times blocks change
	double total = 0.0;
	TMatrix<int> previousboardstate;
	previousboardstate.SetBounds(1,N,1,N);
	previousboardstate.FillContents(0.0);
	// Save latest state
	for (int i = 1; i <= N; i++){
		for (int j = 1; j <= N; j++){
			previousboardstate[i][j] = board.states[i][j];
		}
	}

	// Write to file
	ofstream fout("states.dat");
	for (int i = 1; i <= N; i++)
	{
		for (int j = 1; j <= N; j++)
		{
			fout << board.states[i][j] << " ";
		}
	}
	fout << endl;

	// Simulate for DUR steps
	for (int t = 1; t <= DUR; t++){
		board.Step();

		for (int i = 1; i <= N; i++)
		{
			for (int j = 1; j <= N; j++)
			{
				fout << board.states[i][j] << " ";
			}
		}
		fout << endl;

		double portion = 0.0;
		// Analyze difference and count
		for (int i = 1; i <= N; i++){
			for (int j = 1; j <= N; j++){
				if (board.states[i][j] != previousboardstate[i][j]){
					portion += 1.0;
				}
			}
		}
		total += portion/(N*N);
		// Save latest state
		for (int i = 1; i <= N; i++){
			for (int j = 1; j <= N; j++){
				previousboardstate[i][j] = board.states[i][j];
			}
		}
	}
	fout.close();
	cout << total/DUR << endl;
	return total/DUR;
}

// ------------------------------------
// Display functions
// ------------------------------------
void EvolutionaryRunDisplay(int Generation, double BestPerf, double AvgPerf, double PerfVar)
{
	cout << Generation << " " << BestPerf << " " << AvgPerf << " " << PerfVar << endl;
}

void ResultsDisplay(BSearch &s)
{
	TVector<int> bestVector;
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
	long randomseed = static_cast<long>(time(NULL));
	if (argc == 2)
		randomseed += atoi(argv[1]);

	BSearch s(VectSize);
	
	#ifdef PRINTOFILE
	ofstream file;
	file.open("evol.dat");
	cout.rdbuf(file.rdbuf());
	// save the seed to a file
	ofstream seedfile;
	seedfile.open ("seed.dat");
	seedfile << randomseed << endl;
	seedfile.close();
	#endif
	
	// Configure the search
	s.SetRandomSeed(randomseed);
	s.SetSearchResultsDisplayFunction(ResultsDisplay);
	s.SetPopulationStatisticsDisplayFunction(EvolutionaryRunDisplay);
	s.SetSelectionMode(RANK_BASED);
	s.SetReproductionMode(GENETIC_ALGORITHM);
	s.SetPopulationSize(POPSIZE);
	s.SetMaxGenerations(GENS);
	s.SetCrossoverProbability(CROSSPROB);
	s.SetCrossoverMode(UNIFORM);
	s.SetMutationVariance(MUTVAR);
	s.SetMaxExpectedOffspring(EXPECTED);
	s.SetElitistFraction(ELITISM);
	s.SetSearchConstraint(1);
	
	s.SetEvaluationFunction(FitnessFunction); 
	s.ExecuteSearch();
	
	RecordBehavior(s.BestIndividual());
	return 0;
}
