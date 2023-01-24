#include "Quant.hpp"


QuantBallPosX::QuantBallPosX(int num_bins) : num_bins(num_bins), haxball(){}


QuantBallPosX::QuantBallPosX(fs::path path_to_agent_folder){
    fs::path quantisation_config_file = combine_paths(path_to_agent_folder, "QuantBallPosX.info");
    std::vector<double> config = load_std_vector_from_file(quantisation_config_file);

    num_bins = config[0];
}


int QuantBallPosX::getQuantisation(const Eigen::Ref<const Eigen::VectorXd>& state) const{
    double pos_x = state[2];
    double max_x = haxball.getSize().x();
    return getScalarQuantisation(pos_x, -max_x, max_x, num_bins);
}


int QuantBallPosX::getStateSize() const{
    return num_bins;
}


std::string QuantBallPosX::getType() const{
    std::string type = "QuantBallPosX";
    return type;
}


void QuantBallPosX::saveConfiguration(fs::path path_to_agent_folder) const{
    fs::path quantisation_config_file = combine_paths(path_to_agent_folder, "QuantBallPosX.info");
    std::vector<double> config;
    config.push_back(num_bins);
    save_std_vector_to_file(quantisation_config_file, config);
}




