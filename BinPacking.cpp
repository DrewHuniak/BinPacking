/* 
    By Drew Huniak
    A program that compares the different types of bin packing strategies
    (Next fit, First fit, Best fit, First fit decreasing, and Best fit decreasing)
    and then prints them to a file.
*/
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <iomanip>
using namespace std;

/* Runs the tests and prints the results to a file */
void runTest(ofstream &stream, ofstream &stream2);

/* Prints the average results of all of the tests.*/
void printAverage(ofstream &stream, vector<double> &vect);

/* Can print either the online results or all of the results depending on the string inputed and the vector size .*/
void printOnlAll(ofstream &stream, vector<int> &vect, string choice);

/* Prints the dividing lines for the test results */
void printLine(ofstream &stream);

/* Generates random numbers and stores them in the given vector */
void generateRandom(vector<int> &vect, ofstream &stream);

/* Performs a next fit bin packing stategy on vector of 
   random numbers and returns the amount of bins created. */
int findNextFit(vector<int> &vect, ofstream &stream);

/* Performs a first fit bin packing   stategy on vector of 
   random numbers and returns the amount of bins created. */
int findFirstFit(vector<int> &vect, ofstream &stream);

/* Performs a best fit bin packing stategy on vector of 
   random numbers and returns the amount of bins created. */
int findBestFit(vector<int> &vect, ofstream &stream);

/* Method to help print the bins */
void printBin(ofstream &stream, vector<vector<int>> &bin);

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        cout<<"Insufficent arguments provided, a seed must be provided."<<endl;
        return 1;
    }

    int randSeed = atoi(argv[1]);

    if(randSeed)
    {
        srand(randSeed);
    }
    else
    {
        srand(time(nullptr));     
    }

    string rawData = "rawdata_" + to_string(randSeed) + ".txt"; //needs to be changed later
    ofstream stream(rawData);

    string result = "results_"+to_string(randSeed) + ".txt";
    ofstream stream2(result);

    runTest(stream, stream2);
    
    stream.close();
    stream2.close();
    return 0;
}

void runTest(ofstream &stream, ofstream &stream2)
{
    vector<int> randomNumbers(100);

    //Used for average
    double nextFitTotal = 0.0;
    double firstFitTotal = 0.0;
    double bestFitTotal = 0.0;
    double firstFitDecreasingTotal = 0.0;
    double bestFitDecreasingTotal = 0.0;

    //Counters to keep track of the best case fit for online only.
    int nextCounter = 0;
    int firstCounter = 0;
    int bestCounter = 0;
    
    //Counters to keep track of the best case for all methods.
    int allNext = 0;
    int allFirst = 0;
    int allBest = 0;
    int allFFDec = 0;
    int allBFDec = 0;

    //Keeps track of the bin size for each run.
    int nextFit;
    int firstFit;
    int bestFit;
    int firstFitDecreasing;
    int bestFitDecreasing;

    stream2<<" Exp#"<<setw(8)<<"Next"<<setw(8)<<"First"<<setw(8)<<"Best"<<setw(8)<<"FFDec"<<setw(8)<<"BFDec"<<endl;
    printLine(stream2);
    for(int i = 1; i <= 10; i++)
    {
        stream<<"*Experiment #"<<i<<"*"<<endl;
        
        generateRandom(randomNumbers, stream);
        
        stream<<"Next Fit:"<<endl;
        nextFit = findNextFit(randomNumbers, stream);
        nextFitTotal += nextFit;
        stream<<endl;
       
        stream<<"First Fit:"<<endl;
        firstFit = findFirstFit(randomNumbers, stream);
        firstFitTotal += firstFit;
        stream<<endl;

        stream<<"Best Fit:"<<endl;
        bestFit = findBestFit(randomNumbers, stream);
        bestFitTotal += bestFit;
        stream<<endl;

        sort(randomNumbers.begin(), randomNumbers.end(), greater<int>());
        
        stream<<"First Fit Decreasing:"<<endl;
        firstFitDecreasing = findFirstFit(randomNumbers, stream);
        firstFitDecreasingTotal += firstFitDecreasing;
        stream<<endl;
       
        stream<<"Best Fit Decreasing:"<<endl;
        bestFitDecreasing = findBestFit(randomNumbers, stream);
        bestFitDecreasingTotal += bestFitDecreasing;

        stream<<setprecision(1);
        stream2<<setw(4)<<i<<setw(8)<<nextFit<<setw(8)<<firstFit<<setw(8)<<bestFit<<setw(8)<<firstFitDecreasing<<setw(8)<<bestFitDecreasing<<endl;

        //Finding the best of the online solutions
        vector<int> best = {nextFit, firstFit, bestFit}; 
        sort(best.begin(), best.end());
        int min = best[0];
     
        if(min == nextFit)
        {
            nextCounter++;
        }
        if(min == firstFit)
        {
            firstCounter++;
        }
        if(min == bestFit)
        {
            bestCounter++;
        }

        //Finding the best of all Solutions.
        best.push_back(firstFitDecreasing);
        best.push_back(bestFitDecreasing);
        sort(best.begin(), best.end());
        min = best[0];

        if(min == nextFit)
        {
            allNext++;
        }
        if(min == firstFit)
        {
            allFirst++;
        }
        if(min == bestFit)
        {
            allBest++;
        }
        if(min == firstFitDecreasing)
        {
            allFFDec++;
        }
        if(min == bestFitDecreasing)
        {
            allBFDec++;
        }

        if(i != 10) //Removes the extra new line at the end
        {
            stream<<endl;
        }
    }

    //Print Average
    vector<double> avg = {nextFitTotal, firstFitTotal, bestFitTotal, firstFitDecreasingTotal, bestFitDecreasingTotal};
    printLine(stream2);
    printAverage(stream2, avg);

    //Print Online best performance
    vector<int> onl = {nextCounter, firstCounter, bestCounter};
    printLine(stream2);
    printOnlAll(stream2, onl, "ONL:");

    //Print All best performance
    vector<int> all = {allNext, allFirst, allBest, allFFDec, allBFDec};
    printLine(stream2);
    printOnlAll(stream2, all, "ALL:");
}

void printAverage(ofstream &stream, vector<double> &vect)
{
    stream<<setw(5)<<"AVG:";
    for(int i = 0; i < vect.size(); i++)
    {
        if(i == 0)
        {
            stream<<setw(8)<<setprecision(1)<<fixed<<vect[i]/10;
        }
        else
        {
            stream<<setw(8)<<vect[i]/10;
        }
    }
    stream<<endl;
}

void printOnlAll(ofstream &stream, vector<int> &vect, string choice)
{
    stream<<setw(5)<<choice;
    for(int i = 0; i < vect.size(); i++)
    {
        if(i == 0)
        {
            stream<<setw(7)<<vect[i];
        }
        else
        {
            stream<<setw(8)<<vect[i];
        }
    }
    if(vect.size() == 3)
    {
        stream<<setw(8)<<"--";
        stream<<setw(8)<<"--";
    }
    stream<<endl;
}

void printLine(ofstream &stream)
{
    for(int i = 0; i < 6; i++)
    {
        stream<<"------"<<setw(8);
    }
    stream<<endl;
}

void generateRandom(vector<int> &vect, ofstream &stream)
{
    for(int i = 0; i < vect.size(); i++)
    {
        int random = (rand() % 100) + 1;
        vect[i] = random;
        stream<<vect[i]<<endl;
    }
    stream<<endl;
}

int findNextFit(vector<int> &vect, ofstream &stream)
{
    vector<vector<int>> bin;
    int binCapacity = 0;
    
    vector<int> temp;
    for(int i = 0; i < vect.size(); i++)
    {
        if((vect[i] + binCapacity) > 100)
        {
            bin.push_back(temp);
            temp.clear();
            temp.push_back(vect[i]);
            binCapacity = vect[i];
        }
        else
        {
            temp.push_back(vect[i]);
            binCapacity += vect[i];
        }

        stream<<vect[i]<<" "<<bin.size()<<endl;
    }

    if(temp[0]) //Get the remaining numbers that dont go into a bin.
    {
        bin.push_back(temp);
    }

    stream<<endl;

    //Print each bin and the contents of the bit along as the total.
    int total = 0; 
    for(int i = 0; i < bin.size(); i++)
    {
        stream<<i<<" ";
        for(int j = 0; j < bin[i].size() ; j++)
        {
            total += bin[i][j];
            stream<<bin[i][j]<<" ";
        }
        stream<<total<<endl;
        total = 0;
    }

    return bin.size();    
}

int findFirstFit(vector<int> &vect, ofstream &stream)
{
    //Initialize the first bin with 0 value.
    vector<vector<int>> bin;
    vector<int> temp(1);

    bin.push_back(temp);
    
    bool newBin;

    for(int i = 0; i < vect.size(); i++)
    {
        newBin = true;
        
       
        for(int j = 0; j < bin.size(); j++) // Look for space in the existing bins.
        {
            if((vect[i] + bin[j][0]) <= 100)
            {
                bin[j].push_back(vect[i]);
                bin[j][0] += vect[i];       //Total is stored in the first index 0.
                newBin = false;
                stream<<vect[i]<<" "<<j<<endl;
                j = bin.size();
            }   
        }

        if(newBin) //New bin must be created.
        {
            temp[0] = vect[i]; //update size
            
            bin.push_back(temp);
            bin[bin.size() - 1].push_back(vect[i]); // Place vect[i] in bin[i][1] spot
            
            stream<<vect[i]<<" "<<bin.size() - 1<<endl;
        }
    }

    stream<<endl;

    printBin(stream, bin);

    return bin.size();
}

int findBestFit(vector<int> &vect, ofstream &stream)
{
    vector<vector<int>> bin;
    vector<int> temp(1);
    bin.push_back(temp);

    int bestFit;
    int bestIndex;
    int fit;

    for(int i = 0; i < vect.size(); i++)
    {
        bestFit = 100;
        bestIndex = -1;
        for(int j = 0; j < bin.size(); j++)
        {
            if((vect[i] + bin[j][0]) <= 100)
            {
                fit = 100 - (vect[i] + bin[j][0]);
                if(fit < bestFit)
                {
                    bestFit = fit;
                    bestIndex = j;
                }
            }
        }
        if(bestIndex != -1) 
        {
            bin[bestIndex].push_back(vect[i]);
            bin[bestIndex][0] += vect[i];       //The first value in the bin is the total amount stored in the bin.
            stream<<vect[i]<<" "<<bestIndex<<endl;
        }
        else //create new bin
        {
            temp[0] = vect[i];
            bin.push_back(temp);
            bin[bin.size() - 1].push_back(vect[i]);
            stream<<vect[i]<<" "<<bin.size() -1 <<endl;   
        }
    }

    stream<<endl;

    //Printing bin info
    printBin(stream, bin);

    return bin.size();
}

void printBin(ofstream &stream, vector<vector<int>> &bin)
{
    for(int i = 0; i < bin.size(); i++)
    {
        stream<<i<<" ";
        
        for(int j = 1; j < bin[i].size(); j++)
        {
            stream<<bin[i][j]<<" ";
        }
        stream<<bin[i][0]<<endl;
    }
}