#include "Quant.hpp"


QuantVelocityY::QuantVelocityY(int num_bins) : num_bins(num_bins), haxball(){}


QuantVelocityY::QuantVelocityY(fs::path path_to_agent_folder){
    fs::path quantisation_config_file = combine_paths(path_to_agent_folder, "QuantVelocityY.info");
    std::vector<double> config = load_std_vector_from_file(quantisation_config_file);

    num_bins = config[0];
}


int QuantVelocityY::getQuantisation(const Eigen::Ref<const Eigen::VectorXd>& state) const{
    double vel_y = state[5];
    double max_vel = haxball.getMaxSpeedBall();
    return getScalarQuantisation(vel_y, -max_vel, max_vel, num_bins);
}


int QuantVelocityY::getStateSize() const{
    return num_bins;
}


std::string QuantVelocityY::getType() const{
    std::string type = "QuantVelocityY";
    return type;
}


void QuantVelocityY::saveConfiguration(fs::path path_to_agent_folder) const{
    fs::path quantisation_config_file = combine_paths(path_to_agent_folder, "QuantVelocityY.info");
    std::vector<double> config;
    config.push_back(num_bins);
    save_std_vector_to_file(quantisation_config_file, config);
}




