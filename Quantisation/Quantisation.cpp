#include "Quantisation.hpp"

Quantisation::Quantisation(std::vector<Quant*> quants_){
    for(unsigned int i=0; i<quants_.size(); i++){
        quants.push_back(quants_[i]);
    }
}


Quantisation::Quantisation(fs::path path_to_agent_folder){
    fs::path quantisation_config_file = combine_paths(path_to_agent_folder, "quantisation.info");

    std::ifstream config_file_stream(quantisation_config_file);
    std::string line;

    while (getline(config_file_stream, line)){
        Quant* quant;


        if(line == "QuantVelocity"){
            quant = new QuantVelocity(path_to_agent_folder); 
            quants.push_back(quant);
        }else if(line == "QuantVelocityX"){
            quant = new QuantVelocityX(path_to_agent_folder); 
            quants.push_back(quant);
        }else if(line == "QuantVelocityY"){
            quant = new QuantVelocityY(path_to_agent_folder); 
            quants.push_back(quant);
        }else if(line == "QuantPlayerPosX"){
            quant = new QuantPlayerPosX(path_to_agent_folder); 
            quants.push_back(quant);
        }else if(line == "QuantPlayerPosY"){
            quant = new QuantPlayerPosY(path_to_agent_folder); 
            quants.push_back(quant);
        }else if(line == "QuantBallPosX"){
            quant = new QuantBallPosX(path_to_agent_folder); 
            quants.push_back(quant);
        }else if(line == "QuantBallPosY"){
            quant = new QuantBallPosY(path_to_agent_folder); 
            quants.push_back(quant);
        }else if(line == "QuantAngleBallToPlayer"){
            quant = new QuantAngleBallToPlayer(path_to_agent_folder); 
            quants.push_back(quant);
        }else if(line == "QuantAngleBallToGoal"){
            quant = new QuantAngleBallToGoal(path_to_agent_folder); 
            quants.push_back(quant);
        }else if(line == "QuantAnglePlayerToGoal"){
            quant = new QuantAnglePlayerToGoal(path_to_agent_folder); 
            quants.push_back(quant);
        }else if(line == "QuantDistanceBallToGoal"){
            quant = new QuantDistanceBallToGoal(path_to_agent_folder); 
            quants.push_back(quant);
        }else if(line == "QuantDistanceBallToPlayer"){
            quant = new QuantDistanceBallToPlayer(path_to_agent_folder); 
            quants.push_back(quant);
        }else if(line == "QuantBallBehindAgent"){
            quant = new QuantBallBehindAgent(path_to_agent_folder); 
            quants.push_back(quant);
        }else if(line == "QuantAngleBallToPlayerComplex"){
            quant = new QuantAngleBallToPlayerComplex(path_to_agent_folder); 
            quants.push_back(quant);
        }else{
            std::cout << "Unknown Quant: " << line << std::endl; 
        }
    }
}


int Quantisation::getQtableStateIndex(const Eigen::Ref<const Eigen::VectorXd>& state) const{
    int index = 0;

    for(unsigned int i=0; i<quants.size(); i++){
        int index_multiplier = 1;

        for(unsigned int j=0; j<i; j++){
            index_multiplier *= quants[j]->getStateSize();
        }

        index += index_multiplier * quants[i]->getQuantisation(state);
    }

    return index;
}


int Quantisation::getStateSize() const{
    int state_size = 1;

    for(unsigned int i=0; i<quants.size(); i++){
        state_size *= quants[i]->getStateSize();
    }

    return state_size;
}


std::string Quantisation::getType() const{
    std::string type = "Quantisation";
    return type;
}


std::vector<double> Quantisation::getConfiguration() const{
    std::vector<double> vec;
    return vec;
}


void Quantisation::saveConfiguration(fs::path path_to_agent_folder){
    fs::path quantisation_config_file = combine_paths(path_to_agent_folder, "quantisation.info");

    std::string config;
    for(unsigned int i=0; i<quants.size(); i++){
        config += quants[i]->getType() + "\n";
    }

    std::ofstream info_file_stream(quantisation_config_file);
    info_file_stream << config;

    info_file_stream.close();

    for(unsigned int i=0; i<quants.size(); i++){
        quants[i]->saveConfiguration(path_to_agent_folder);
    }
}