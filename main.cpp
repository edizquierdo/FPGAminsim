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
const int N = 3; 

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
	long randomseed = static_cast<long>(time(NULL));
	if (argc == 2)
		randomseed += atoi(argv[1]);

	TSearch s(VectSize);
	
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
	
	return 0;
}
