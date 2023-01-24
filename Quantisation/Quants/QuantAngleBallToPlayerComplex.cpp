#include "Quant.hpp"


QuantAngleBallToPlayerComplex::QuantAngleBallToPlayerComplex(int num_angles_forward, int num_angles_backward, bool extra_feature_ball_behind_agent) 
        : num_angles_forward(num_angles_forward), num_angles_backward(num_angles_backward), extra_feature_ball_behind_agent(extra_feature_ball_behind_agent), haxball(){}


QuantAngleBallToPlayerComplex::QuantAngleBallToPlayerComplex(fs::path path_to_agent_folder){
    fs::path quantisation_config_file = combine_paths(path_to_agent_folder, "QuantAngleBallToPlayerComplex.info");
    std::vector<double> config = load_std_vector_from_file(quantisation_config_file);

    num_angles_forward = config[0];
    num_angles_backward = config[1];
    extra_feature_ball_behind_agent = config[2];
}


int QuantAngleBallToPlayerComplex::getQuantisation(const Eigen::Ref<const Eigen::VectorXd>& state) const{
    double player_x = state[0];
    double player_y = state[1];
    double ball_x = state[2];
    double ball_y = state[3];
    double rel_x = ball_x - player_x;
    double rel_y = ball_y - player_y;


    double angle = getAngle_rad(rel_x, rel_y);

    int quant = 0;

    if(angle >= 0){
        quant = getScalarQuantisation(angle, 0, M_PI, num_angles_forward);
    }else if(angle < 0){
        quant = num_angles_forward + getScalarQuantisation(angle, -M_PI, 0, num_angles_backward);
    }

    if(extra_feature_ball_behind_agent){
        if(abs(player_y - ball_y) < (haxball.getRadiusBall() + haxball.getRadiusPlayer())){   // Ball is directly behind agent
            quant = num_angles_forward + num_angles_backward;
        }
    }

    return quant;
}


int QuantAngleBallToPlayerComplex::getStateSize() const{
    return num_angles_forward + num_angles_backward + extra_feature_ball_behind_agent;
}


std::string QuantAngleBallToPlayerComplex::getType() const{
    std::string type = "QuantAngleBallToPlayerComplex";
    return type;
}


void QuantAngleBallToPlayerComplex::saveConfiguration(fs::path path_to_agent_folder) const{
    fs::path quantisation_config_file = combine_paths(path_to_agent_folder, "QuantAngleBallToPlayerComplex.info");
    std::vector<double> config;
    config.push_back(num_angles_forward);
    config.push_back(num_angles_backward);
    config.push_back(extra_feature_ball_behind_agent);
    save_std_vector_to_file(quantisation_config_file, config);
}




