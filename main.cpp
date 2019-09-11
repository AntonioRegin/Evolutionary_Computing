#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>

using namespace std;
using namespace std;


double eval(int pj[]);


const int SolutionSize = 100;
//in case of 1 index arrays
const int LastIndex =  SolutionSize -1;
const int FirstIndex = 0;
//max results possible after calling eval
const double MaxPossibleResult = 100;
//number of evaluations calls the program has done
int evaluationsCalls=0;

class  Solution{
public:

    int vector[100];
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
        for(int i=0;i<SolutionSize;i++)
            file<< " " << vector[i];
        file<<"\n";
    }

};

//making sure the evals calls are properly counted
void Evaluation(Solution &solution){
    evaluationsCalls++;
    solution.fitness = eval(solution.vector);
}


//my hill climber
class  Optimizer{
public:
    Solution BestSolution;
    ofstream log;

private:
    int currentPosition;
    int maxEvaluations;
    Solution newSolution;

public:
    explicit Optimizer(int seed, int maxEvaluations,const char *logName) : BestSolution(), newSolution() {
        srand (seed);
        currentPosition =FirstIndex;
        this->maxEvaluations = maxEvaluations;
        log.open(logName);
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
        log << evaluationsCalls<<" ";
        BestSolution.PrintToFile(log);
    }
    //randamly initialized a vector in the domain space {0,1}
    void InitSolution(Solution &solution){
        for(int i=0;i<SolutionSize;i++)
            solution.vector[i] = (rand()%2);
        Evaluation(solution);
        Log();
    }
    //avance one step in hamming distance
    void Modify(const Solution &solution, Solution &newSolution){
        newSolution.CopyFromOther(solution);
        int  p = GetAndAdvancePosition();
        newSolution.vector[p] = (solution.vector[p] == 0) ? 1 : 0;
        Evaluation(newSolution);
    }
    //climbs the hill advancing 1 step in hamming distance at a time
    //stops if maxSolution found or runs out of cycles
    bool Climb(Solution &solution, Solution &tmp, int maxCyclesWithoutImprovement) {
        int cyclesWithoutImprovement = 0;
        if (maxCyclesWithoutImprovement < SolutionSize)
            maxCyclesWithoutImprovement = SolutionSize;
        while (true) {
            cyclesWithoutImprovement++;
            Modify(BestSolution, newSolution);
            //new solution is at least as good as last swap
            if (newSolution.fitness >= BestSolution.fitness) {
                if(newSolution.fitness > BestSolution.fitness)
                    cyclesWithoutImprovement=0;
                BestSolution.CopyFromOther(newSolution);
                Log();
            }
            if (BestSolution.fitness >= MaxPossibleResult) {
                return true;
            }
            if (evaluationsCalls >= maxEvaluations) {
                return false;
            }
            if (cyclesWithoutImprovement >= maxCyclesWithoutImprovement) {
                return false;
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
        Evaluation(newSolution);
    }
    //randomly creates a new soltion to test
    void Transmute(Solution &solution){
        for(int i=0;i<SolutionSize;i++)
            solution.vector[i] = (rand()%2);
        Evaluation(solution);
    }

public:
    void Optimize(){
        InitSolution(BestSolution);
        while (evaluationsCalls < maxEvaluations && BestSolution.fitness < MaxPossibleResult) {
            bool maxSolutionFound = Climb(BestSolution,newSolution,1000);
            if (maxSolutionFound == false) {
                bool betterSolutiontFound = false;
                int i=0;
                while (evaluationsCalls < maxEvaluations && betterSolutiontFound == false) {
                    Jump(BestSolution,newSolution, i);
                    if(newSolution.fitness>BestSolution.fitness)
                        betterSolutiontFound= true;
                    if(newSolution.fitness>=BestSolution.fitness) {
                        BestSolution.CopyFromOther(newSolution);
                        Log();
                    }
                    if (betterSolutiontFound == false) {
                        Transmute(newSolution);
                        if(newSolution.fitness>BestSolution.fitness)
                            betterSolutiontFound= true;
                        if(newSolution.fitness>=BestSolution.fitness) {
                            BestSolution.CopyFromOther(newSolution);
                            Log();
                        }
                    }
                }
            }
        }
    }

};

void ResetEnviroment(){
    evaluationsCalls=0;
}

// Function to generate all binary strings
void ExploreAllSpace(int n, int arr[], int i)
{
    if (i == n) {
        Solution s;
        for(int i=0;i<30;i++){
            s.vector[i] = arr[i];
        }
        for(int i=30;i<50;i++){
            s.vector[i]=1;
        }
        for(int i=50;i<100;i++){
            s.vector[i]=0;
        }
        Evaluation(s);
        if(s.fitness>70) {
            //vector is better that max solution in hill climber
            s.PrintToConsole();
            cout<<"Number of Evaluation calls: "<<evaluationsCalls<<endl;
            exit(0);
        }
        return;
    }

    // First assign "0" at ith position
    // and try for all other permutations
    // for remaining positions
    arr[i] = 0;
    ExploreAllSpace(n, arr, i + 1);

    // And then assign "1" at ith position
    // and try for all other permutations
    // for remaining positions
    arr[i] = 1;
    ExploreAllSpace(n, arr, i + 1);
}


int main()
{
    cout<<"*******************************************************************************"<<endl;
    cout<<"Climb the hill"<<endl;
    cout<<"Runs: 1";

    int f1 = 0, f2 = 1;
    int f3 = f1 + f2;
    Solution best;
    double  avgFitness=0;
    double minFitness;
    double avgCalls=0;
    int minCalls;
    int  maxCalls;

    //first calls to optimezer
    Optimizer optimizer(f3, 100000, "HillClimberLog");
    f1 = f2;f2 = f3;f3 = f1+f2;
    optimizer.Optimize();
    best.CopyFromOther(optimizer.BestSolution);
    avgFitness+=optimizer.BestSolution.fitness;
    minFitness = optimizer.BestSolution.fitness;
    avgCalls += evaluationsCalls;
    minCalls = evaluationsCalls;
    maxCalls = evaluationsCalls;
    ResetEnviroment();
    //29 calls to optimizer
    for(int i=0;i<29;i++) {
        cout<<", "<<(i+2);
        optimizer.Reset(f3, 100000);
        f1 = f2;f2 = f3;f3 = f1+f2;
        optimizer.Optimize();
        if(optimizer.BestSolution.fitness>best.fitness) {
            best.CopyFromOther(optimizer.BestSolution);
        }
        if(optimizer.BestSolution.fitness<minFitness)
            minFitness = optimizer.BestSolution.fitness;
        avgFitness+=optimizer.BestSolution.fitness;
        if(evaluationsCalls> maxCalls)
            maxCalls = evaluationsCalls;
        if(evaluationsCalls<minCalls)
            minCalls = evaluationsCalls;
        avgCalls += evaluationsCalls;
        ResetEnviroment();
    }
    cout<<endl;
    avgFitness/=30;
    avgCalls/=30;
    cout<<"*******************************************************************************"<<endl;
    cout<<"Hill Climber, Number of calls: 30"<<endl;
    cout<<"Best Solution"<<endl;
    best.PrintToConsole();
    cout<<"Average Fitness: "<< avgFitness <<endl;
    cout<<"Min Fitness: "<< minFitness <<endl;
    cout<<"Average Number of Evaluations: "<< avgCalls <<endl;
    cout<<"Max Number of Evaluations: "<< maxCalls <<endl;
    cout<<"Min Number of Evaluations: "<< minCalls <<endl;


    cout<<"*******************************************************************************"<<endl;



    if(best.fitness==70){
        cout<<"*******************************************************************************"<<endl;
        cout<<"Exploring first 30 bits of eval1"<<endl;
        int n = 30;
        int arr[n];
        ExploreAllSpace(n,arr,0);
    }
    if(best.fitness<=60){
        cout<<"*******************************************************************************"<<endl;
        cout<<"Solution for evalN"<<endl;
        Solution s;
        //377
        s.vector[83] = 1;s.vector[85] = 0;s.vector[87] = 1;s.vector[89] = 1;s.vector[91] = 1;s.vector[93] = 1;s.vector[95] = 0;s.vector[97] = 0;s.vector[99] = 1;
        Evaluation(s);
        s.PrintToConsole();

    }

    return 0;
}

