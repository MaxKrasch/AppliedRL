#include "greediness.hpp"
#include <vector>
using namespace std;

Greediness::Greediness(){
    cases=EPSILON_NOT_INITIALIZED;
}

double Greediness::getProbabilityForRandomAction(int epoch, int timestep){
    if(cases==EPSILON_NOT_INITIALIZED){
        std::cout << "Error: greediness is not initialized";
        return 1;
    }
    switch (cases){
        case CONSTANT_EPSILON:
        {
            return epsilon;
        }
        case LOG_EPSILON_TIMESTEP:
        {
            double prob = prob_end + (offset-prob_end)*exp(-decay_parameter*epoch);
            return prob;
        }
        case LINEAR_EPSILON:
        {
            double prob = offset - epoch * decay_parameter;
            return prob;
        }
    }
    std::cout << "Error: Unknown greediness case";
    return 1;
}

bool Greediness::useSameAction(){
    if(repeat_action > action_count){
        action_count++;
        return true;
    }
    if(action_count == repeat_action){
        action_count = 0;
    }
    return false;
}

void Greediness::initialiseConstantEpsilon(double epsilon){
    this->epsilon = epsilon;

    cases = CONSTANT_EPSILON;
}

void Greediness::initialiseLogEpsilonTimestep(double start_probability, double end_probability, int epochs, int repeat_action, double target){
    this->decay_parameter = decay_parameter;
    double epoch_target = epochs * target;  // 90000
    decay_parameter = 11.51/epoch_target;   // -ln(0.00001)=11.51 --> after epoch_target is reached: prob = prob_end+0.00001
    offset = start_probability;
    prob_end = end_probability;

    cases = LOG_EPSILON_TIMESTEP;
}

void Greediness::initialiseLinearEpsilon(double start_probability, double end_probability, int epochs, int repeat_action){
    this->repeat_action = repeat_action;
    action_count = 0;

    offset = start_probability;
    double delta = start_probability - end_probability;
    decay_parameter = delta / (epochs-1);    

    cases = LINEAR_EPSILON;
}

std::string Greediness::getInfo(){
    std::string string;
    //TODO
    return string;
}
