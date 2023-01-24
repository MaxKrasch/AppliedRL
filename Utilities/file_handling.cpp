#include "file_handling.hpp"


fs::path combine_paths(fs::path path, std::string filename)
{
    path.append(filename);
    return path;
}

Eigen::VectorXd load_vectorxd_from_file(fs::path filepath){
    std::vector<double> std_vec;

    std::ifstream file_stream(filepath);

    double value=0.0;
    while(file_stream >> value){
        std_vec.push_back(value);
    }

    Eigen::VectorXd vec_xd = Eigen::Map<Eigen::VectorXd, Eigen::Unaligned>(std_vec.data(), std_vec.size());

    return vec_xd;
}


bool save_vectorxd_to_file(fs::path filepath, Eigen::VectorXd vec_xd){
    std::remove(filepath.c_str());  // remove file if it already exists

    std::ofstream file_stream(filepath);

    for(int i=0; i<vec_xd.size(); i++){
        file_stream << vec_xd[i] << "\n";
    }

    file_stream.close();
    return true;
}


std::vector<double> load_std_vector_from_file(fs::path filepath){
    std::vector<double> std_vec;

    std::ifstream file_stream(filepath);

    double value=0.0;
    while(file_stream >> value){
        std_vec.push_back(value);
    }

    return std_vec;
}


bool save_std_vector_to_file(fs::path filepath, std::vector<double> std_vec){
    std::remove(filepath.c_str());  // remove file if it already exists

    std::ofstream file_stream(filepath);

    for(unsigned int i=0; i<std_vec.size(); i++){
        file_stream << std_vec[i] << "\n";
    }

    file_stream.close();
    return true;
}
