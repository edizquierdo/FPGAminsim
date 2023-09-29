// ***********************************************************
// A class for a minimal simulation of an FPGA
// v1. Instatenouos (i.e., no delays)
// ************************************************************

#include "VectorMatrix.h"
#include "random.h"
#include <iostream>
#include <math.h>

#pragma once

// The FPGA class declaration

class FPGA {
    public:
        // The constructor
        FPGA(int newsize = 0);
        // The destructor
        ~FPGA();
        
        // Accessors
        int BlockSize(void) {return size;};
        void SetBlockSize(int newsize);

        double BlockState(int i, int j) {return states[i][j];};
        void SetBlockState(int i, int j, double value) {states[i][j] = value;};
        
        double BlockGate(int i, int j) {return gates[i][j];};
        void SetBlockGate(int i, int j, double value) {gates[i][j] = value;};

        double InputAx(int i, int j) {return Ax[i][j];};
        void SetInputAx(int i, int j, double value) {Ax[i][j] = value;}; 

        double InputAy(int i, int j) {return Ay[i][j];};
        void SetInputAy(int i, int j, double value) {Ay[i][j] = value;};        

        double InputBx(int i, int j) {return Bx[i][j];};
        void SetInputBx(int i, int j, double value) {Bx[i][j] = value;};        

        double InputBy(int i, int j) {return By[i][j];};
        void SetInputBy(int i, int j, double value) {By[i][j] = value;};        

        // Input and output
        friend ostream& operator<<(ostream& os, FPGA& c);
        // friend istream& operator>>(istream& is, FPGA& c);
                            
        // Control
        void ZeroBlockState();
        void OneBlockState();
        void RandomizeBlockState();
        void RandomizeBlockState(RandomState &rs);
        void UpdateHistory();
        void Step();
		
        int size;
        int maxdelay;
        TMatrix<int> states, history, gates, Ax, Ay, Bx, By; // delays
};

