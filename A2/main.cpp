
#include "Solution.cpp"
#include "HillClimber.cpp"

int Solution::NumberOfEvalutionCalls =0;

static void ResetEnvironment(){
    Solution::NumberOfEvalutionCalls=0;
}
int main()
{
    ResetEnvironment();
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
    HillClimber optimizer(f3, 1000, "./HillClimberLog");
    f1 = f2;f2 = f3;f3 = f1+f2;
    optimizer.Optimize();
    cout<<"run 1 : eval calls: "<<Solution::NumberOfEvalutionCalls<<endl;
    optimizer.BestSolution.PrintToConsole();
    best.CopyFromOther(optimizer.BestSolution);
    avgFitness=optimizer.BestSolution.fitness;
    minFitness = optimizer.BestSolution.fitness;
    avgCalls = Solution::NumberOfEvalutionCalls;
    minCalls = Solution::NumberOfEvalutionCalls;
    maxCalls = Solution::NumberOfEvalutionCalls;
    ResetEnvironment();
    //29 calls to optimizer
    for(int i=0;i<29;i++) {
        cout<<""<<endl;
        optimizer.Reset(f3, 1000);
        f1 = f2;f2 = f3;f3 = f1+f2;
        optimizer.Optimize();
        cout<<"run "<< i+2<<" : eval calls: "<<Solution::NumberOfEvalutionCalls<<endl;
        optimizer.BestSolution.PrintToConsole();
        if(optimizer.BestSolution.fitness>best.fitness) {
            best.CopyFromOther(optimizer.BestSolution);
        }
        if(optimizer.BestSolution.fitness<minFitness)
            minFitness = optimizer.BestSolution.fitness;
        avgFitness+=optimizer.BestSolution.fitness;
        if(Solution::NumberOfEvalutionCalls> maxCalls)
            maxCalls = Solution::NumberOfEvalutionCalls;
        if(Solution::NumberOfEvalutionCalls<minCalls)
            minCalls = Solution::NumberOfEvalutionCalls;
        avgCalls += Solution::NumberOfEvalutionCalls;

        ResetEnvironment();
    }
    cout<<endl;
    avgFitness/=30;
    avgCalls/=30;
    cout<<"*******************************************************************************"<<endl;
    cout<<"Hill Climber, Number of calls: 30"<<endl;
    cout<<"Best Solution"<<endl;
    best.PrintToConsole();
    cout<<"Average Max Fitness: "<< avgFitness <<endl;
    cout<<"Min Fitness: "<< minFitness <<endl;
    cout<<"Average Number of Evaluations: "<< avgCalls <<endl;
    cout<<"Max Number of Evaluations: "<< maxCalls <<endl;
    cout<<"Min Number of Evaluations: "<< minCalls <<endl;


    cout<<"*******************************************************************************"<<endl;



    return 0;
}

