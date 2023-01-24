#include "Quant.hpp"


QuantVelocityX::QuantVelocityX(int num_bins) : num_bins(num_bins), haxball(){}


QuantVelocityX::QuantVelocityX(fs::path path_to_agent_folder){
    fs::path quantisation_config_file = combine_paths(path_to_agent_folder, "QuantVelocityX.info");
    std::vector<double> config = load_std_vector_from_file(quantisation_config_file);

    num_bins = config[0];
}


int QuantVelocityX::getQuantisation(const Eigen::Ref<const Eigen::VectorXd>& state) const{
    double vel_x = state[4];
    double max_vel = haxball.getMaxSpeedBall();
    return getScalarQuantisation(vel_x, -max_vel, max_vel, num_bins);
}


int QuantVelocityX::getStateSize() const{
    return num_bins;
}


std::string QuantVelocityX::getType() const{
    std::string type = "QuantVelocityX";
    return type;
}


void QuantVelocityX::saveConfiguration(fs::path path_to_agent_folder) const{
    fs::path quantisation_config_file = combine_paths(path_to_agent_folder, "QuantVelocityX.info");
    std::vector<double> config;
    config.push_back(num_bins);
    save_std_vector_to_file(quantisation_config_file, config);
}




