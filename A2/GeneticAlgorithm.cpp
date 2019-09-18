//
// Created by mithr on 9/17/19.
//

# include <cstdlib>
# include <iostream>
# include <iomanip>
# include <fstream>
# include <iomanip>
# include <cmath>
# include <ctime>
# include <cstring>
#include <stdlib.h>
#include <vector>
#include <algorithm>

using namespace std;

double eval(int pj[]);

#ifndef GA_
#define GA_

const int PopulationSize = 100;
const int NumberOfGnerations = 200;
const int  GenesSize = 150;
const int NumElitism = 1;
const double  ProbabailityOfCrossOver = 0.7;
const double ProbabilityOfMutation = 0.001;
int NumberOfEvaluationCalls =0;

class GenerationStatistics{
public:
    double maxFitness;
    double minFitness;
    double  avgFitness;
    GenerationStatistics(){
        double maxFitness=0;
        double minFitness=0;
        double  avgFitness=0;
    }
};
class GlobalStatistics{
public:
    std::vector<GenerationStatistics *> generations;
    int currentIndividualsAcounted;
    GenerationStatistics *currentGeneration;
    GlobalStatistics(){
        currentIndividualsAcounted=0;
        currentGeneration = new GenerationStatistics();
        generations.push_back(currentGeneration);
    }
    ~GlobalStatistics(){
        for(int i=0;i<generations.size();i++)
            delete generations[i];
    }
    void AddFitnessToGeneration(double fitness){
        if(currentIndividualsAcounted > PopulationSize)
            throw "individuals more than population size";
        if(currentIndividualsAcounted==0){
            currentGeneration->avgFitness = fitness;
            currentGeneration->maxFitness = fitness;
            currentGeneration->minFitness=fitness;
        } else {
            if (currentGeneration->maxFitness < fitness)
                currentGeneration->maxFitness = fitness;
            if (currentGeneration->minFitness > fitness)
                currentGeneration->minFitness = fitness;
            currentGeneration->avgFitness += fitness;
        }
        currentIndividualsAcounted++;
    }
    GenerationStatistics* LastValidGeneration(){
        if(generations.size()>1)
        return generations[generations.size()-2];
        else
            return nullptr;
    }
    void CloseGeneration(){
        if(currentIndividualsAcounted != PopulationSize)
            throw "individuals are un-accounted";
        currentGeneration->avgFitness /= PopulationSize;
        currentGeneration = new GenerationStatistics();
        generations.push_back(currentGeneration);
        currentIndividualsAcounted=0;
    }
    void Reset(){
        for(int i=0;i<generations.size();i++)
            delete generations[i];
        generations.resize(0);
        currentIndividualsAcounted=0;
        currentGeneration = new GenerationStatistics();
        generations.push_back(currentGeneration);
    }

};

//  Each Individual is a member of the population, with
//  genes: the 0,1 solution vector of geneSize 150
class Individual
{
public:
    int genes[GenesSize];
    double fitness;
    double probabilityOfSelection;
    double startOfPie;
    double endOfPie;
    Individual(){
        for(int i=0;i<GenesSize;i++)
            genes[i]=0;
        fitness = 0;
        probabilityOfSelection = 0;
        startOfPie = 0;
        endOfPie = 0;
    }
    void CopyFromOther(const Individual &other){
        memcpy(genes , other.genes , GenesSize*sizeof(int));
        fitness = other.fitness;
        probabilityOfSelection = other.probabilityOfSelection;
        startOfPie = other.startOfPie;
        endOfPie = other.endOfPie;
    }
    void CopyFromVector(const int vec[]){
        memcpy(genes , vec , GenesSize*sizeof(int));
    }

    void PrintToConsole(){
        cout<<"Fitness: "<<fitness<<endl;
        cout<<"Solution: "<<endl;
        cout<< "[ "<<genes[0];
        for(int i=1;i<GenesSize;i++)
            cout<< " " << genes[i];
        cout<<" ]" <<endl;
    }
    void PrintToFile(ofstream &file){
        file<<fitness;
        for(int i=0;i<GenesSize;i++)
            file<< " " << genes[i];
        file<<"\n";
    }
    //making sure the evals calls are properly counted
    void Evaluate(){
        NumberOfEvaluationCalls++;
        fitness = eval(genes);
    }

};
bool Compare(Individual *a,Individual *b){
    return (a->fitness>b->fitness);
}

class StandardGeneticAlgorithm{
public:
    std::vector<Individual *> population;
    std::vector<Individual *> tmpStorage;
    GlobalStatistics gt;
    ofstream log;

    StandardGeneticAlgorithm(const char *logName) {
        for (int i = 0; i < PopulationSize; i++) {
            population.push_back(new Individual());
            tmpStorage.push_back(new Individual());
        }
        log.open(logName);
    }
    ~StandardGeneticAlgorithm(){
        log.close();
    }

private:


    //initialization of a random population
    void Initialize ( ) {
        Individual *member = population[0];
        for (int g = 0; g < GenesSize; g++)
            member->genes[g] = 0;
        member = population[1];
        for (int g = 0; g < GenesSize; g++)
            member->genes[g] = 1;
        for (int i = 2; i < PopulationSize; i++) {
            member = population[i];
            for (int g = 0; g < GenesSize; g++)
                member->genes[g] = (rand() % 2);
        }
    }
    void Elitism(){
        for(int i=0;i<NumElitism;i++)
            tmpStorage[i]->CopyFromOther(*population[i]);
    }
//    Select one individual at random.
//returns the index os the selected individual
    void Select (){
        Elitism();
        for(int ind=NumElitism;ind<PopulationSize;ind++) {
            double x = ((double) rand()) / RAND_MAX;
            if (x < 0 || x > 1)
                throw "Something wrong with the c++ rand()";
            bool found = false;
            int i =0;
            while (found == false) {
                    Individual *member = population[i];
                    if (i == (PopulationSize-1) ||  (x >= member->startOfPie && x < member->endOfPie)) {
                        if(i==(PopulationSize-1))
                            found=false;
                        found = true;
                        tmpStorage[ind]->CopyFromOther(*member);
                    }
                    i++;
            }
        }
    }

    //    CROSSOVER selects two parents for the single point crossover.
    void Crossover ( int parentA[], int parentB[], int genesA[], int genesB[] ){

        int crossOverPoint = rand() % GenesSize;

        int i;
        for ( i = 0; i <= crossOverPoint; i++ )
        {
            genesA[i] = parentA[i];
            genesB[i] = parentB[i];
        }
        for ( i = crossOverPoint+1; i < GenesSize; i++ )
        {
            genesA[i] = parentB[i];
            genesB[i] = parentA[i];
        }
    }

//   copy genes with probability of mutation mutation.
    void CopyWithProbabilityOfMutation ( int genes[], int genesCopy[]  ) {
        for (int i = 0; i < GenesSize; i++) {
            double x = ((double) rand()) / RAND_MAX;
            if (x < ProbabilityOfMutation)
                genesCopy[i] = (genes[i] == 0) ? 1 : 0;
            else
                genesCopy[i] = genes[i];
        }
    }

    //re-evaluates the fitness of the population
    void Evaluate (){
        Individual *member;
        double t_fitness =0;
        for ( int i = 0; i < PopulationSize; i++ )
        {
            member = population[i];
            member->Evaluate();
         //   if(member->fitness<=0)
           //     member->PrintToConsole();
            member->probabilityOfSelection = 0;
            member->startOfPie = 0;
            member->endOfPie = 0;
            gt.AddFitnessToGeneration(member->fitness);
            t_fitness+=member->fitness;
        }
        gt.CloseGeneration();
        sort(population.begin(),population.end(),Compare);
        double previousEndOfPie = 0;
        for ( int i = 0; i < PopulationSize; i++ )
        {
            member = population[i];
            member->probabilityOfSelection = member->fitness/ t_fitness;
            member->startOfPie = previousEndOfPie;
            member->endOfPie = previousEndOfPie + member->probabilityOfSelection;
            previousEndOfPie = member->endOfPie;
        }
    }
    void CopyGenes(Individual *ind,int genesCopy[]){
        memcpy(genesCopy, ind->genes , GenesSize*sizeof(int));
    }
    void CopyGenes(int genes[],int genesCopy[]){
        memcpy(genesCopy, genes , GenesSize*sizeof(int));
    }
    void Log(){
        GenerationStatistics *gs = gt.LastValidGeneration();
        log<<gt.generations.size()-1<<" "<<NumberOfEvaluationCalls<<" "<<gs->maxFitness<<" "<<gs->avgFitness<<" "<<gs->minFitness<<endl;
    }

public:
    void PrintToScreen(){
        GenerationStatistics *gs = gt.LastValidGeneration();
        cout<<gt.generations.size()-1<<"\t\t   "<<NumberOfEvaluationCalls<<"  \t\t"<<gs->maxFitness<<"\t\t   "<<gs->avgFitness<<"\t\t"<<gs->minFitness<<endl;
    }
    void Optimize(int seed,int numberOfGenerations){
        srand(seed);
        Initialize();
        Evaluate();
        Log();
        int tmp_A[GenesSize];
        int tmp_B[GenesSize];
        int final_A[GenesSize];
        int final_B[GenesSize];
        int ind =0;
        for(int i=1;i<numberOfGenerations;i++){
            Select();
            for(int p =NumElitism;p<PopulationSize;){
                double x = ((double) rand()) / RAND_MAX;
                if(x<ProbabailityOfCrossOver && p<(PopulationSize-1)) {
                    Crossover( tmpStorage[p]->genes,tmpStorage[p+1]->genes,tmp_A,tmp_B);
                    CopyWithProbabilityOfMutation(tmp_A,final_A);
                    CopyWithProbabilityOfMutation(tmp_B,final_B);
                    population[p]->CopyFromVector(final_A);
                    population[p+1]->CopyFromVector(final_B);
                    p+=2;
                }        else{
                    CopyWithProbabilityOfMutation(tmpStorage[p]->genes,final_A);
                    population[p]->CopyFromVector(final_A);
                    p+=1;
                }
            }
            Evaluate();
            Log();
        }
    }
    void Reset(){
        NumberOfEvaluationCalls=0;
        gt.Reset();
    }

};

int main ( );
void report ( int generation );
void Select (int &seed );
void timestamp ( );

//****************************************************************************80

int main ( ){



        cout << "\n";
        cout << "Run      Generation       # Evaluation          Best               Average               Min \n";
        cout << "Number    number             Calls              Fitness            fitness             Fitness \n";
        cout << "\n";

    int f1 = 0, f2 = 1;
    int f3 = f1 + f2;
    double avgMaxFitness=0;
    double avgAvgFitness=0;
    double avgMinFitness=0;
    Individual best;
    StandardGeneticAlgorithm ga("./GALog");
    for(int i=0;i<30;i++) {
        ga.Optimize(f3,NumberOfGnerations);
        if(i==0)
            best.CopyFromOther(*ga.population[0]);
        else{
            if(best.fitness<ga.population[0]->fitness)
                best.CopyFromOther(*ga.population[0]);
        }
        avgMaxFitness+=ga.gt.LastValidGeneration()->maxFitness;
        avgAvgFitness+=ga.gt.LastValidGeneration()->avgFitness;
        avgMinFitness+=ga.gt.LastValidGeneration()->minFitness;
        cout<<(i+1)<<" \t   ";
        ga.PrintToScreen();
        ga.Reset();
        f1 = f2;f2 = f3;f3 = f1+f2;
    }

    cout<<endl;
    avgMaxFitness/=30;
    avgMinFitness/=30;
    avgAvgFitness/=30;
    cout<<"*******************************************************************************"<<endl;
    cout<<"GA, Number of calls: 30"<<endl;
    cout<<"Best Solution"<<endl;
    best.PrintToConsole();
    cout<<"Average Max Fitness: "<< avgMaxFitness <<endl;
    cout<<"Average Average Fitness: "<< avgAvgFitness <<endl;
    cout<<"Average Min Fitness: "<< avgMinFitness <<endl;

    cout<<"*******************************************************************************"<<endl;

    return 0;
}



#endif