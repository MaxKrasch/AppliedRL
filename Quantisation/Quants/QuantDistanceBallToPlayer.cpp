#include "Quant.hpp"


QuantDistanceBallToPlayer::QuantDistanceBallToPlayer(std::vector<double> bin_borders_) : haxball(){
    for(unsigned int i=0; i<bin_borders_.size(); i++){
        bin_borders.push_back(bin_borders_[i]);
    }
}


QuantDistanceBallToPlayer::QuantDistanceBallToPlayer(fs::path path_to_agent_folder){
    fs::path quantisation_config_file = combine_paths(path_to_agent_folder, "QuantDistanceBallToPlayer.info");
    std::vector<double> loaded_vec = load_std_vector_from_file(quantisation_config_file);
    for(unsigned int i=0; i<loaded_vec.size(); i++){
        bin_borders.push_back(loaded_vec[i]);
    }
}


int QuantDistanceBallToPlayer::getQuantisation(const Eigen::Ref<const Eigen::VectorXd>& state) const{
    double player_x = state[0];
    double player_y = state[1];
    
    double ball_x = state[2];
    double ball_y = state[3];

    double distance_x = player_x - ball_x;
    double distance_y = player_y - ball_y;

    double distance = sqrt(pow(distance_x, 2) + pow(distance_y, 2));

    int quant = 0;
    
    for(unsigned int i=0; i<bin_borders.size(); i++){
        if(distance < bin_borders[i]){
            return quant;
        }
        quant++;
    }
    return quant;
}


int QuantDistanceBallToPlayer::getStateSize() const{
    return bin_borders.size() + 1;
}


std::string QuantDistanceBallToPlayer::getType() const{
    std::string type = "QuantDistanceBallToPlayer";
    return type;
}


void QuantDistanceBallToPlayer::saveConfiguration(fs::path path_to_agent_folder) const{
    fs::path quantisation_config_file = combine_paths(path_to_agent_folder, "QuantDistanceBallToPlayer.info");
    save_std_vector_to_file(quantisation_config_file, bin_borders);
}




