#include "Quant.hpp"


QuantAngleBallToGoal::QuantAngleBallToGoal(int num_bins) : num_bins(num_bins), haxball(){}


QuantAngleBallToGoal::QuantAngleBallToGoal(fs::path path_to_agent_folder){
    fs::path quantisation_config_file = combine_paths(path_to_agent_folder, "QuantAngleBallToGoal.info");
    std::vector<double> config = load_std_vector_from_file(quantisation_config_file);

    num_bins = config[0];
}


int QuantAngleBallToGoal::getQuantisation(const Eigen::Ref<const Eigen::VectorXd>& state) const{
    double angle = getAngleFromGoal_rad(state[2], state[3]);
    return getScalarQuantisation(angle, 0.0, M_PI, num_bins);

}


int QuantAngleBallToGoal::getStateSize() const{
    return num_bins;
}


std::string QuantAngleBallToGoal::getType() const{
    std::string type = "QuantAngleBallToGoal";
    return type;
}


void QuantAngleBallToGoal::saveConfiguration(fs::path path_to_agent_folder) const{
    fs::path quantisation_config_file = combine_paths(path_to_agent_folder, "QuantAngleBallToGoal.info");
    std::vector<double> config;
    config.push_back(num_bins);
    save_std_vector_to_file(quantisation_config_file, config);
}




