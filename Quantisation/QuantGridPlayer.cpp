#include "QuantGridPlayer.hpp"

QuantGridPlayer::QuantGridPlayer(int quant_size_x, int quant_size_y) : 
    quant_size_x_(quant_size_x), quant_size_y_(quant_size_y), hax_env_()
{
    
}


int QuantGridPlayer::getQtableStateIndex(const Eigen::Ref<const Eigen::VectorXd>& state) const{
    double x = state[0];
    double y = state[1];
    x -= hax_env_.getSize().x(); // now x is in the range [0, 2*size_x]
    y -= hax_env_.getSize().y(); // now y is in the range [0, 2*size_y]

    // quantize x
    double x_step = (-2 * hax_env_.getSize().x()) / (double)quant_size_x_;
    int x_quantized = (int) (x / x_step);    

    if(x_quantized == quant_size_x_){
        x_quantized--;
    }

    // quantize y
    double y_step = (-2 * hax_env_.getSize().y()) / (double)quant_size_y_;
    int y_quantized = (int) (y / y_step);

    if(y_quantized == quant_size_y_){
        y_quantized--;
    }


    return (y_quantized * quant_size_x_) + x_quantized;
}


int QuantGridPlayer::getStateSize() const{
    return quant_size_x_ * quant_size_y_;
}

std::string QuantGridPlayer::getType() const{
    return "QuantGridPlayer";
}

std::vector<double> QuantGridPlayer::getConfiguration() const{
    std::vector<double> vec;
    vec.push_back(quant_size_x_);
    vec.push_back(quant_size_y_);
    return vec;
}