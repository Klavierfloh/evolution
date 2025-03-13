import random
import random as rand


def gene(howMuch):
    gene = []
    for i in range(howMuch):
        gene.append((rand.random() * 10, rand.randint(10, 1000)))

    return gene

gegenstaende = gene(20000)
grenzgewichtRucksack = 15.0

# Funktionen zur Erzeugung einer Lösung

anzahlIndividuen = 1000
mutationswahrscheinlichkeit = 0.05

def erzeugeIndividuum(zahl):
    # Erzeugt eine binäre Darstellung der Zahl
    individuum = bin(zahl)[2:]
    # Stellt sicher, dass die Länge der binären Darstellung mit der Anzahl der Gegenstände übereinstimmt
    while len(individuum) < len(gegenstaende):
        individuum = '0' + individuum
    return individuum

def erzeugePopulation():
    zahlenRangePop = 2 ** len(gegenstaende)
    population = []
    for i in range(anzahlIndividuen):
        # Zahl zwischen 0 und zahlenRangePop erzeugen, passend zur Anzahl der Gegenstände
        population.append(erzeugeIndividuum(rand.randint(0, zahlenRangePop - 1)))
    return population

def fitness(individuum):
    individuum = str(individuum)
    fitness = 0
    for i, digit in enumerate(individuum):
        if int(digit) == 1:
            gObjekt, oFitness = gegenstaende[i]
            fitness += oFitness
    return fitness

def kreuzung(individuum1, individuum2):
    # Kreuzungspunkt zwischen den beiden Individuen
    placeToCut = rand.randint(0, len(gegenstaende) - 1)
    neuesIndividuum1 = individuum1[:placeToCut] + individuum2[placeToCut:]
    neuesIndividuum2 = individuum2[:placeToCut] + individuum1[placeToCut:]
    return (neuesIndividuum1, neuesIndividuum2)

def mutation(individuum):
    # Wählt einen zufälligen Index für die Mutation
    indexToChange = rand.randint(0, len(gegenstaende) - 1)
    individuum_list = list(individuum)

    # Umkehrung des Werts an der bestimmten Stelle
    if individuum_list[indexToChange] == '1':
        individuum_list[indexToChange] = '0'
    else:
        individuum_list[indexToChange] = '1'

    # Zurück zum String
    individuum = ''.join(individuum_list)
    return individuum

def selektionElternteil(population):
    # Wählt zufällig zwei Individuen aus der Population aus
    ind1 = rand.randint(0, len(population) - 1)
    ind2 = rand.randint(0, len(population) - 1)
    while ind2 == ind1:
        ind2 = rand.randint(0, len(population) - 1)

    elternteilPos1 = population[ind1]
    elternteilPos2 = population[ind2]
    if fitness(elternteilPos1) > fitness(elternteilPos2):
        elternteil = elternteilPos1
    else:
        elternteil = elternteilPos2

    return elternteil

def naechstePopulation(population):
    neuePopulation = []
    for i in range(anzahlIndividuen // 2):  # Muss durch 2 geteilt werden, da Paare erzeugt werden
        # Kreuzung der Eltern
        eltern1 = selektionElternteil(population)
        eltern2 = selektionElternteil(population)
        neuesIndividuum1, neuesIndividuum2 = kreuzung(eltern1, eltern2)

        # Mutation anwenden
        if rand.random() <= mutationswahrscheinlichkeit:
            neuesIndividuum1 = mutation(neuesIndividuum1)
        if rand.random() <= mutationswahrscheinlichkeit:
            neuesIndividuum2 = mutation(neuesIndividuum2)

        neuePopulation.append(neuesIndividuum1)
        neuePopulation.append(neuesIndividuum2)
    return neuePopulation

def maxFitness(population):
    maxIndividuum = 0
    maximumFitness = 0
    for i in population:
        f = fitness(i)
        if f > maximumFitness:
            maximumFitness = f
            maxIndividuum = i

    return maxFitness#(maximumFitness,maxIndividuum)

def loesungGenetischerAlgorithmus():
    population = erzeugePopulation()
    print("Maximale Fitness vor Evolution: " + str(maxFitness(population)))
    for i in range(100):
        population = naechstePopulation(population)
    print("Maximale Fitness nach Evolution: " + str(maxFitness(population)))
    return population

loesungGenetischerAlgorithmus()

