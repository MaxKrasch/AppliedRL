#include "Quant.hpp"


QuantAnglePlayerToGoal::QuantAnglePlayerToGoal(int num_bins) : num_bins(num_bins), haxball(){}


QuantAnglePlayerToGoal::QuantAnglePlayerToGoal(fs::path path_to_agent_folder){
    fs::path quantisation_config_file = combine_paths(path_to_agent_folder, "QuantAnglePlayerToGoal.info");
    std::vector<double> config = load_std_vector_from_file(quantisation_config_file);

    num_bins = config[0];
}


int QuantAnglePlayerToGoal::getQuantisation(const Eigen::Ref<const Eigen::VectorXd>& state) const{
    double angle = getAngleFromGoal_rad(state[0], state[1]);
    return getScalarQuantisation(angle, 0.0, M_PI, num_bins);

}


int QuantAnglePlayerToGoal::getStateSize() const{
    return num_bins;
}


std::string QuantAnglePlayerToGoal::getType() const{
    std::string type = "QuantAnglePlayerToGoal";
    return type;
}


void QuantAnglePlayerToGoal::saveConfiguration(fs::path path_to_agent_folder) const{
    fs::path quantisation_config_file = combine_paths(path_to_agent_folder, "QuantAnglePlayerToGoal.info");
    std::vector<double> config;
    config.push_back(num_bins);
    save_std_vector_to_file(quantisation_config_file, config);
}




