#include "Quant.hpp"


QuantPlayerPosX::QuantPlayerPosX(int num_bins) : num_bins(num_bins), haxball(){}


QuantPlayerPosX::QuantPlayerPosX(fs::path path_to_agent_folder){
    fs::path quantisation_config_file = combine_paths(path_to_agent_folder, "QuantPlayerPosX.info");
    std::vector<double> config = load_std_vector_from_file(quantisation_config_file);

    num_bins = config[0];
}


int QuantPlayerPosX::getQuantisation(const Eigen::Ref<const Eigen::VectorXd>& state) const{
    double pos_x = state[0];
    double max_x = haxball.getSize().x();
    return getScalarQuantisation(pos_x, -max_x, max_x, num_bins);
}


int QuantPlayerPosX::getStateSize() const{
    return num_bins;
}


std::string QuantPlayerPosX::getType() const{
    std::string type = "QuantPlayerPosX";
    return type;
}


void QuantPlayerPosX::saveConfiguration(fs::path path_to_agent_folder) const{
    fs::path quantisation_config_file = combine_paths(path_to_agent_folder, "QuantPlayerPosX.info");
    std::vector<double> config;
    config.push_back(num_bins);
    save_std_vector_to_file(quantisation_config_file, config);
}




