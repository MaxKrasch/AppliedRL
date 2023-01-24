#include "Quant.hpp"


QuantBallBehindAgent::QuantBallBehindAgent(){}


QuantBallBehindAgent::QuantBallBehindAgent(fs::path path_to_agent_folder){
}


int QuantBallBehindAgent::getQuantisation(const Eigen::Ref<const Eigen::VectorXd>& state) const{
    double player_x = state[0];
    double player_y = state[1];
    double ball_x = state[2];
    double ball_y = state[3];

    if (abs(player_y - ball_y) < (haxball.getRadiusBall() + haxball.getRadiusPlayer()))
    {
        if (player_x - ball_x > 0)
        {
            return 1;
        }
    }
    return 0;
}


int QuantBallBehindAgent::getStateSize() const{
    return 2;
}


std::string QuantBallBehindAgent::getType() const{
    std::string type = "QuantBallBehindAgent";
    return type;
}


void QuantBallBehindAgent::saveConfiguration(fs::path path_to_agent_folder) const{}




