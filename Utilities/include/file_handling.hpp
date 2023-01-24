#ifndef _FILE_HANDLING_H_
#define _FILE_HANDLING_H_



#include "Eigen/Dense"

// Standard Library
#include <string.h>
#include <fstream>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;


/**
 * @brief Combines a path and a filename to get the path to the file
 * 
 * @param path The path to the folder of the file
 * @param filename The name of the file
 * @return fs::path The path to the file
 */
fs::path combine_paths(fs::path path, std::string filename);    

Eigen::VectorXd load_vectorxd_from_file(fs::path filepath);

bool save_vectorxd_to_file(fs::path filepath, Eigen::VectorXd vec_xd);

std::vector<double> load_std_vector_from_file(fs::path filepath);

bool save_std_vector_to_file(fs::path filepath, std::vector<double> std_vec);


#endif // _FILE_HANDLING_H_