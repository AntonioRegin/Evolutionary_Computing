//
// Created by mithr on 9/17/19.
//


//my hill climber

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include "Solution.cpp"
#include "Globals.cpp"

using namespace std;

#ifndef HC_
#define HC_


class  HillClimber{
public:
    Solution BestSolution;
    ofstream log;

private:
    int currentPosition;
    int maxEvaluations;
    Solution newSolution;

public:
    explicit HillClimber(int seed, int maxEvaluations,const char *logName) : BestSolution(), newSolution() {
        srand (seed);
        currentPosition =FirstIndex;
        this->maxEvaluations = maxEvaluations;
        log.open(logName);
    }
    ~HillClimber(){
        log.close();
    }
    void Reset(int seed, int maxEvaluations){
        srand (seed);
        currentPosition =FirstIndex;
        this->maxEvaluations = maxEvaluations;
    }

private:
    int GetAndAdvancePosition(){
        int rs = currentPosition;
        if(currentPosition == (LastIndex))
            currentPosition=FirstIndex;
        else
            currentPosition++;
        return  rs;
    }
    //log best solution
    void Log(){
        log <<Solution::NumberOfEvalutionCalls<<" ";
        BestSolution.PrintToFile(log);

    }
    //randamly initialized a vector in the domain space {0,1}
    void InitSolution(Solution &solution){
        for(int i=0;i<SolutionSize;i++)
            solution.vector[i] = (rand()%2);
        solution.Evaluate();
        Log();
    }
    //avance one step in hamming distance
    void Modify(const Solution &solution, Solution &newSolution){
        newSolution.CopyFromOther(solution);
        int  p = GetAndAdvancePosition();
        newSolution.vector[p] = (solution.vector[p] == 0) ? 1 : 0;
        newSolution.Evaluate();
    }
    //climbs the hill advancing 1 step in hamming distance at a time
    //stops if maxSolution found or runs out of cycles
    void Climb(Solution &solution, Solution &tmp, int maxCyclesWithoutImprovement) {
        int cyclesWithoutImprovement = 0;
        if (maxCyclesWithoutImprovement < SolutionSize)
            maxCyclesWithoutImprovement = SolutionSize;
        while (true) {
            cyclesWithoutImprovement++;
            Modify(BestSolution, newSolution);
            //new solution is at least as good as last swap
            if (newSolution.fitness >= BestSolution.fitness) {
                if(newSolution.fitness > BestSolution.fitness) {
                    cyclesWithoutImprovement = 0;
                    BestSolution.CopyFromOther(newSolution);
                    Log();
                } else
                    BestSolution.CopyFromOther(newSolution);

            }
            if (Solution::NumberOfEvalutionCalls >= maxEvaluations) {
                return;
            }
            if (cyclesWithoutImprovement >= maxCyclesWithoutImprovement) {
                return;
            }
        }
    }
    //randomly flips n bits, with replacement of the index
    //in other words the bit can be flip and the flip again
    void Jump(const Solution &solution, Solution &newSolution,int jumpSize){
        newSolution.CopyFromOther(solution);
        for(int i = 0;i<jumpSize;i++){
            int position = rand() % 100;
            newSolution.vector[position] = (solution.vector[position] == 0)? 1 : 0;
        }
        newSolution.Evaluate();
    }
    //randomly creates a new solution to test
    void Transmute(Solution &solution){
        for(int i=0;i<SolutionSize;i++)
            solution.vector[i] = (rand()%2);
        solution.Evaluate();
    }

public:
    void Optimize(){
        log<<endl;
        InitSolution(BestSolution);
        while (Solution::NumberOfEvalutionCalls < maxEvaluations) {
            Climb(BestSolution,newSolution,160);
            Log();
            if (Solution::NumberOfEvalutionCalls < maxEvaluations) {
                log<<"Start jumping at: "<<Solution::NumberOfEvalutionCalls<<endl;
                bool betterSolutiontFound = false;
                int i=0;
                while (Solution::NumberOfEvalutionCalls < maxEvaluations && betterSolutiontFound == false) {
                    Jump(BestSolution,newSolution, i);
                    if(newSolution.fitness>BestSolution.fitness)
                        betterSolutiontFound= true;
                    if(newSolution.fitness>=BestSolution.fitness) {
                        BestSolution.CopyFromOther(newSolution);
                    }
                    if (betterSolutiontFound == false) {
                        Transmute(newSolution);
                        if(newSolution.fitness>BestSolution.fitness)
                            betterSolutiontFound= true;
                        if(newSolution.fitness>=BestSolution.fitness) {
                            BestSolution.CopyFromOther(newSolution);
                        }
                    }
                    if(betterSolutiontFound) {
                        Log();
                        log<<"End jumping at: "<<Solution::NumberOfEvalutionCalls<<endl;
                    }
                }
            }
        }
        Log();
        log<<endl;
    }

};

#endif