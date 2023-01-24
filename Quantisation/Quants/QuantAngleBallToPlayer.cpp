#include "Quant.hpp"


QuantAngleBallToPlayer::QuantAngleBallToPlayer(int num_bins) : num_bins(num_bins), haxball(){}


QuantAngleBallToPlayer::QuantAngleBallToPlayer(fs::path path_to_agent_folder){
    fs::path quantisation_config_file = combine_paths(path_to_agent_folder, "QuantAngleBallToPlayer.info");
    std::vector<double> config = load_std_vector_from_file(quantisation_config_file);

    num_bins = config[0];
}


int QuantAngleBallToPlayer::getQuantisation(const Eigen::Ref<const Eigen::VectorXd>& state) const{
    double rel_x = state[2] - state[0];
    double rel_y = state[3] - state[1];

    double angle = getAngle_rad(rel_x, rel_y);

    return getScalarQuantisation(angle, -M_PI, M_PI, num_bins);
}


int QuantAngleBallToPlayer::getStateSize() const{
    return num_bins;
}


std::string QuantAngleBallToPlayer::getType() const{
    std::string type = "QuantAngleBallToPlayer";
    return type;
}


void QuantAngleBallToPlayer::saveConfiguration(fs::path path_to_agent_folder) const{
    fs::path quantisation_config_file = combine_paths(path_to_agent_folder, "QuantAngleBallToPlayer.info");
    std::vector<double> config;
    config.push_back(num_bins);
    save_std_vector_to_file(quantisation_config_file, config);
}




