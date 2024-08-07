#include "BSearch.h"
#include "FPGA.h"
#include "random.h"

//#define PRINTOFILE

// Task params
const double DUR = 100; 

// EA params
const int POPSIZE = 96;
const int GENS = 100; //100;
const double MUTVAR = 0.05;			
const double CROSSPROB = 0.0;
const double EXPECTED = 1.1;
const double ELITISM = 0.02;
const int REPS = 100;

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

	// Define a counter for the number of times blocks change
	double total = 0.0;
	TMatrix<int> previousboardstate;
	previousboardstate.SetBounds(1,N,1,N);
	previousboardstate.FillContents(0.0);

	for (int r = 1; r <= REPS; r++){
		// Initialize state
		//board.ZeroBlockState();
		board.RandomizeBlockState();

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
			// Analyze difference and count it
			for (int i = 1; i <= N; i++){
				for (int j = 1; j <= N; j++){
					if (board.states[i][j] != previousboardstate[i][j]){
						portion += 1.0;
					}
					// Save latest state
					previousboardstate[i][j] = board.states[i][j];
				}
			}
			total += portion/(N*N);
		}
	}

	return total/(DUR*REPS);
}

// ------------------------------------
// Fitness function
// ------------------------------------
double FitnessFunction2(TVector<int> &g, RandomState &rs)
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

	// Count the number of times ONE block changes
	double total = 0.0;

	// Save latest state
	int previousState = board.states[1][1];

	// Simulate for DUR steps
	for (int t = 1; t <= DUR; t++){
		board.Step();
		if (board.states[1][1] != previousState){
			total += 1.0;
		}
		// Save latest state
		previousState = board.states[1][1];
	}
	total = 1 - ((fabs(0.5 - (total/DUR)))/0.5);
	//cout << total << endl;
	return total;
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
	//board.ZeroBlockState();
	board.RandomizeBlockState();
	
	cout << board << endl;
	
	// Count the number of times blocks change
	double total = 0.0, total2=0.0;
	int previousState = board.states[1][1];
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
		if (board.states[1][1] != previousState){
			total2 += 1.0;
		}
		// Save latest state
		for (int i = 1; i <= N; i++){
			for (int j = 1; j <= N; j++){
				previousboardstate[i][j] = board.states[i][j];
			}
		}
		previousState = board.states[1][1];
	}
	fout.close();
	cout << "T1:\t" <<  total/DUR << endl;
	cout << "T2:\t" << total2/DUR << " " << 1 - ((fabs(0.5 - (total2/DUR)))/0.5) << endl;
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
	s.SetReEvaluationFlag(1);
	
	s.SetEvaluationFunction(FitnessFunction); 
	s.ExecuteSearch();
	
	RecordBehavior(s.BestIndividual());
	return 0;
}
