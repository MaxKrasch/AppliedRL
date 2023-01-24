#include "Quant.hpp"


QuantVelocity::QuantVelocity(int num_bins) : num_bins(num_bins), haxball(){}


QuantVelocity::QuantVelocity(fs::path path_to_agent_folder){
    fs::path quantisation_config_file = combine_paths(path_to_agent_folder, "QuantVelocity.info");
    std::vector<double> config = load_std_vector_from_file(quantisation_config_file);

    num_bins = config[0];
}


int QuantVelocity::getQuantisation(const Eigen::Ref<const Eigen::VectorXd>& state) const{
    double vel = sqrt(pow(state[4],2) + pow(state[5],2));
    double max_vel = sqrt(2 * pow(haxball.getMaxSpeedBall(),2));
    return getScalarQuantisation(vel, -max_vel, max_vel, num_bins);
}


int QuantVelocity::getStateSize() const{
    return num_bins;
}


std::string QuantVelocity::getType() const{
    std::string type = "QuantVelocity";
    return type;
}


void QuantVelocity::saveConfiguration(fs::path path_to_agent_folder) const{
    fs::path quantisation_config_file = combine_paths(path_to_agent_folder, "QuantVelocity.info");
    std::vector<double> config;
    config.push_back(num_bins);
    save_std_vector_to_file(quantisation_config_file, config);
}




