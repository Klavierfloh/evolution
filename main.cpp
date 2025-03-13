#include <iostream>
#include <tuple>
#include <vector>
#include <string>
#include <bitset>
#include <random> //für Random()
#include <cmath>

using namespace std;

random_device rd;
mt19937 gen(rd()); // set up random number generator

vector<tuple<float, int>> gene(int howMuch)
{
    vector<tuple<float, int>> allGenes = {};
    allGenes.reserve(howMuch);

    uniform_real_distribution<float> dist(0.0, 1.0); // Weight
    uniform_int_distribution<int> dust(0, 1000);     // Fitness
    for (int i = 0; i < howMuch; i++)
    {
        tuple<float, int> oneGene = make_tuple(dist(gen) * 10, dust(gen));
        allGenes.push_back(oneGene);
    }

    return allGenes;
}

const int amountOfGenes = 100;
const int amountOfIndividuals = 100;
const vector<tuple<float, int>> alleGene = gene(amountOfGenes); // gewicht , Fitnessboost
const int mutationChance = 7;                                   // in %
const float maxWeight = 2 * amountOfGenes;
const float probDiOfWeigth = 0.999;

string erzeugeIndividuum(int zahl)
{
    string individuum;
    for (int i = amountOfGenes - 1; i >= 0; --i)
    {
        individuum += ((zahl >> i) & 1) ? '1' : '0';
    }

    return individuum;
}

vector<string> generatePopulation()
{
    vector<string> population;
    srand(time(nullptr)); // setzt seed für random
    for (int i = 0; i < amountOfIndividuals; i++)
    {

        float zahlenRangePop = pow(2, amountOfGenes) - 1; // Max possible value for amountOfGenes bits

        uniform_int_distribution<int> dist(0, zahlenRangePop);

        int number = dist(gen); // Generate a truly uniform random number
        population.push_back(erzeugeIndividuum(number));
    }
    if (population.size() <= 1)
    {
        runtime_error("Whole population died :(");
    }
    return population;
}

int fitness(const string &individuum)
{
    int fit = 0;
    for (size_t i = 0; i < individuum.size(); i++)
    {
        if (individuum[i] == '1')
        {
            fit += get<1>(alleGene[i]); // Direct access
        }
    }
    return fit;
}

tuple<int, string> maxFitness(const vector<string> &population)
{
    int maxFit = 0;
    string maxInd = "";

    for (string individuum : population)
    {
        int fit = fitness(individuum);
        if (fit > maxFit)
        {
            maxFit = fit;
            maxInd = individuum;
        }
    }

    return make_tuple(maxFit, maxInd);
}

float weight(const string &individuum)
{
    float weight = 0.0;
    for (int i = 0; i < individuum.size(); i++)
    {
        if (individuum[i] == '1')
        {
            int fit = get<0>(alleGene[i]);
            weight += fit;
        }
    }

    return weight;
}

void weightClenser(vector<string> &population)
{
    uniform_real_distribution<> dis(0.0, 1.0); // Random float between 0.0 and 1.0

    // Randomly erase individuals based on weight and chance
    for (auto it = population.begin(); it != population.end();)
    {
        // Randomly erase if weight is too high
        if (weight(*it) > maxWeight && dis(gen) < probDiOfWeigth) // 50% chance of removal
        {
            // cout << "Removing with weight: " << weight(*it) << endl;
            it = population.erase(it); // erase returns the next valid iterator
        }
        else
        {
            // cout << "not removing with weight: " << weight(*it) << endl;
            ++it; // Only increment if nothing was erased
        }
    }
}

tuple<string, string> kreuzen(string parent1, string parent2)
{
    string individdum1 = "";
    string individdum2 = "";

    uniform_int_distribution<int> dist(0, amountOfGenes - 1);

    int indexToSplit = dist(gen); // generieren

    individdum1 = parent1.substr(0, indexToSplit) + parent2.substr(indexToSplit);
    individdum2 = parent2.substr(0, indexToSplit) + parent1.substr(indexToSplit);

    if (individdum1.size() != amountOfGenes)
    {
        runtime_error("Too many genes in indiviuum");
    }

    return make_tuple(individdum1, individdum2);
}

string selectParent(const vector<string> &population)
{
    uniform_int_distribution<int> dist(0, population.size() - 1);

    int indexParent1 = dist(gen); // generieren
    int indexParent2 = dist(gen);

    while (indexParent1 == indexParent2)
    {
        indexParent2 = dist(gen);
    }

    string parentCan1 = population[indexParent1];
    string parentCan2 = population[indexParent2];

    if (fitness(parentCan1) > fitness(parentCan2))
    {
        return parentCan1;
    }
    else
        return parentCan2;
}

string mutation(string individuum)
{
    uniform_int_distribution<int> dist(0, amountOfGenes - 1);

    int indexToMutate = dist(gen); // generieren

    if (individuum[indexToMutate] == '1')
    {
        individuum[indexToMutate] = '0';
    }
    else
        individuum[indexToMutate] = '1';

    return individuum;
}

void printPopulation(const vector<string> &pop)
{
    for (int i = 0; i < pop.size(); i++)
    {
        cout << "Fitness: " << fitness(pop[i]) << " With weight: " << weight(pop[i]) << " Genome: " << pop[i] << endl;
    }
}

void printMaxOfPop(const vector<string> &population)
{
    tuple<int, string> max = maxFitness(population);
    cout << "Highest Fitness in current Population: " << get<0>(max) << " With weight: " << weight(get<1>(max)) << endl;
}

void nextPopulation(vector<string> &population)
{
    vector<string> nextPopulation = {};
    uniform_int_distribution<int> dist(0, 100);

    weightClenser(population);

    for (int i = 0; i < amountOfIndividuals / 2; i++)
    {
        string parent1 = selectParent(population);
        string parent2 = selectParent(population);

        tuple<string, string> newInds = kreuzen(parent1, parent2);
        string newInd1 = get<0>(newInds);
        string newInd2 = get<1>(newInds);

        int mutationFacotr = dist(gen);
        if (mutationFacotr < mutationChance)
        {
            newInd1 = mutation(newInd1);
        }
        mutationFacotr = dist(gen);
        if (mutationFacotr < mutationChance)
        {
            newInd2 = mutation(newInd2);
        }
        nextPopulation.push_back(newInd1);
        nextPopulation.push_back(newInd2);
    }
    population.swap(nextPopulation); // Swap to avoid deep copy
}

vector<string> simulateGenerations(const vector<string> &startingPopulation, int amount)
{
    vector<string> initialPop;

    if (startingPopulation.size() == 1 && startingPopulation[0] == "NULL")
    {
        initialPop = generatePopulation();
    }
    else
    {
        initialPop = startingPopulation;
    }

    for (int i = 0; i < amount; i++)
    {
        nextPopulation(initialPop);
    }

    return initialPop;
}

void printHelp()
{
    cout << "Options:\n"
         << " --loading     Shows loading bar \n"
         << " --help       Shows this list" << endl;
}

int main(int argc, char *argv[])
{
    bool loadingBar = false;
    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "--help")
        {
            printHelp();
            return 0;
        }
        else if (arg == "--loading")
        {
            loadingBar = true;
        }
        else
        {
            printHelp();
            return 0;
        }
    }

    int amount;

    cout << "How many generations? : ";
    cin >> amount;

    vector<string> pop = simulateGenerations({"NULL"}, amount / 100);
    printMaxOfPop(pop);

    if (loadingBar)
    {
        int done = 0;
        int barWidth = 70;
        int toDo = max(1, amount / 100);
        float progress = 0.0;

        while (done < amount)
        {
            pop = simulateGenerations(pop, toDo);
            done += toDo;

            if (done > amount)
                done = amount;

            progress = (float)done / amount; // FIX: Correct float division

            // Loading bar display
            std::cout << "[";
            int pos = barWidth * progress;
            for (int i = 0; i < barWidth; ++i)
            {
                if (i < pos)
                    std::cout << "=";
                else if (i == pos)
                    std::cout << ">";
                else
                    std::cout << " ";
            }
            std::cout << "] " << int(progress * 100.0) << " %\r";
            std::cout.flush();
        }
        std::cout << endl; // Finish line break

        printMaxOfPop(pop);
    }
    else
    {
        vector<string> pop = simulateGenerations({"NULL"}, amount);
        printMaxOfPop(pop);
    }

    return 0;
}
