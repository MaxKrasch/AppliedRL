#ifndef GREEDINESS_H
#define GREEDINESS_H

#include "Eigen/Dense"
#include <stdlib.h>  
#include <iostream>
#include <random>

#define EPSILON_NOT_INITIALIZED 0
#define CONSTANT_EPSILON 1
#define LOG_EPSILON_TIMESTEP 3
#define LINEAR_EPSILON 4

class Greediness
{

public:

    Greediness();

    void initialiseConstantEpsilon(double epsilon);
    void initialiseLogEpsilonTimestep(double start_probability, double end_probability, int epochs, int repeat_action, double target);
    void initialiseLinearEpsilon(double start_probability, double end_probability, int epochs, int repeat_action);

    double getProbabilityForRandomAction(int epoch, int timestep);
    
    bool useSameAction();    

    std::string getInfo();

private:

    int cases;      // determines type of epsilon greediness
    double epsilon;
    int epochs;
    int action_count = 0;   // needed for case 2 to determine if an action is determined greedily or copied from the time step before
    double offset=0;
    double decay_parameter;
    int repeat_action;
    double prob_end;
};


#endif /* GREEDINESS_H */