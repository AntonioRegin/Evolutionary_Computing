//
// Created by mithr on 9/17/19.
//

#include <iostream>
#include <string.h>
#include <fstream>
#include "Globals.cpp"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpragma-once-outside-header"
using namespace std;

#ifndef S_
#define S_

double eval(int pj[]);

const int SolutionSize = 150;
//in case of 1 index arrays
const int LastIndex =  SolutionSize -1;
const int FirstIndex = 0;
//number of evaluations calls the program has done






class  Solution{
public:
    static int NumberOfEvalutionCalls;
public:
    int vector[SolutionSize];
    double fitness;

    Solution(){
        for(int i=0;i<SolutionSize;i++)
            vector[i]=0;
    }
    void CopyFromOther(const Solution &other){
        memcpy(vector , other.vector , SolutionSize*sizeof(int));
        fitness = other.fitness;
    }
    void PrintToConsole(){
        cout<<"Fitness: "<<fitness<<endl;
        cout<<"Solution: "<<endl;
        cout<< "[ "<<vector[0];
        for(int i=1;i<SolutionSize;i++)
            cout<< " " << vector[i];
        cout<<" ]" <<endl;
    }
    void PrintToFile(ofstream &file){
        file<<fitness;
//        for(int i=0;i<SolutionSize;i++)
  //          file<< " " << vector[i];
        file<<"\n";
    }
    //making sure the evals calls are properly counted
    void Evaluate(){
        NumberOfEvalutionCalls++;
        fitness = eval(vector);
    }

};

#endif