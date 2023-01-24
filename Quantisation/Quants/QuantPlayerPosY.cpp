#include "Quant.hpp"


QuantPlayerPosY::QuantPlayerPosY(int num_bins) : num_bins(num_bins), haxball(){}


QuantPlayerPosY::QuantPlayerPosY(fs::path path_to_agent_folder){
    fs::path quantisation_config_file = combine_paths(path_to_agent_folder, "QuantPlayerPosY.info");
    std::vector<double> config = load_std_vector_from_file(quantisation_config_file);
    
    num_bins = config[0];
}


int QuantPlayerPosY::getQuantisation(const Eigen::Ref<const Eigen::VectorXd>& state) const{
    double pos_y = state[1];
    double max_y = haxball.getSize().y();
    return getScalarQuantisation(pos_y, -max_y, max_y, num_bins);
}


int QuantPlayerPosY::getStateSize() const{
    return num_bins;
}


std::string QuantPlayerPosY::getType() const{
    std::string type = "QuantPlayerPosY";
    return type;
}


void QuantPlayerPosY::saveConfiguration(fs::path path_to_agent_folder) const{
    fs::path quantisation_config_file = combine_paths(path_to_agent_folder, "QuantPlayerPosY.info");
    std::vector<double> config;
    config.push_back(num_bins);
    save_std_vector_to_file(quantisation_config_file, config);
}




