#include "QuantGridPlayerBall.hpp"

QuantGridPlayerBall::QuantGridPlayerBall(int quant_size_player_x, int quant_size_player_y, int quant_size_ball_x, int quant_size_ball_y) : 
    quant_size_player_x_(quant_size_player_x), quant_size_player_y_(quant_size_player_y), 
    quant_size_ball_x_(quant_size_player_x), quant_size_ball_y_(quant_size_player_y), 
    hax_env_()
{
    
}


int QuantGridPlayerBall::getQtableStateIndex(const Eigen::Ref<const Eigen::VectorXd>& state) const{

    // ********* Quantisation Player Coordinates ****************

    double player_x = state[0];
    double player_y = state[1];
    player_x -= hax_env_.getSize().x(); // now x is in the range [0, 2*size_x]
    player_y -= hax_env_.getSize().y(); // now y is in the range [0, 2*size_y]

    // quantize x
    double player_x_step = (-2 * hax_env_.getSize().x()) / (double)quant_size_player_x_;
    int player_x_quantized = (int) (player_x / player_x_step);    

    if(player_x_quantized == quant_size_player_x_){
        player_x_quantized--;
    }

    // quantize y
    double player_y_step = (-2 * hax_env_.getSize().y()) / (double)quant_size_player_y_;
    int player_y_quantized = (int) (player_y / player_y_step);

    if(player_y_quantized == quant_size_player_y_){
        player_y_quantized--;
    }

    // ********* Quantisation Ball Coordinates ****************

    double ball_x = state[0];
    double ball_y = state[1];
    ball_x -= hax_env_.getSize().x(); // now x is in the range [0, 2*size_x]
    ball_y -= hax_env_.getSize().y(); // now y is in the range [0, 2*size_y]

    // quantize x
    double ball_x_step = (-2 * hax_env_.getSize().x()) / (double)quant_size_ball_x_;
    int ball_x_quantized = (int) (ball_x / ball_x_step);    

    if(ball_x_quantized == quant_size_ball_x_){
        ball_x_quantized--;
    }

    // quantize y
    double ball_y_step = (-2 * hax_env_.getSize().y()) / (double)quant_size_ball_y_;
    int ball_y_quantized = (int) (ball_y / ball_y_step);

    if(ball_y_quantized == quant_size_ball_y_){
        ball_y_quantized--;
    }

    // (x_pos_quantized * (y_quantize * x_quantize * y_quantize)) + (y_pos_quantized * (x_quantize * y_quantize))+(x_pos_quantized_ball*y_quantize)+y_pos_quantized_ball;
    int index = 0;

    index += player_x_quantized * (quant_size_player_y_ * quant_size_ball_x_ * quant_size_ball_y_);
    index += player_y_quantized * (quant_size_ball_x_ * quant_size_ball_y_);
    index += ball_x_quantized * quant_size_ball_y_;
    index += ball_y_quantized;

    return index;
}


int QuantGridPlayerBall::getStateSize() const{
    return quant_size_player_x_ * quant_size_player_y_ * quant_size_ball_x_ * quant_size_ball_y_;
}

std::string QuantGridPlayerBall::getType() const{
    return "QuantGridPlayerBall";
}

std::vector<double> QuantGridPlayerBall::getConfiguration() const{
    std::vector<double> vec;
    vec.push_back(quant_size_player_x_);
    vec.push_back(quant_size_player_y_);
    vec.push_back(quant_size_ball_x_);
    vec.push_back(quant_size_ball_y_);
    return vec;
}