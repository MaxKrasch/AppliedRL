#include "QuantGridPlayerAngleBall.hpp"

QuantGridPlayerAngleBall::QuantGridPlayerAngleBall(int quant_size_x, int quant_size_y) : quant_size_player_x_(quant_size_x), quant_size_player_y_(quant_size_y), hax_env_()
{
}

int QuantGridPlayerAngleBall::getQtableStateIndex(const Eigen::Ref<const Eigen::VectorXd> &state) const
{
    // ********* Quantisation Player Coordinates ****************

    double player_x = state[0];
    double player_y = state[1];
    player_x -= hax_env_.getSize().x(); // now x is in the range [0, 2*size_x]
    player_y -= hax_env_.getSize().y(); // now y is in the range [0, 2*size_y]

    // quantize x
    double player_x_step = (-2 * hax_env_.getSize().x()) / (double)quant_size_player_x_;
    int player_x_quantized = (int)(player_x / player_x_step);

    if (player_x_quantized == quant_size_player_x_)
    {
        player_x_quantized--;
    }

    // quantize y
    double player_y_step = (-2 * hax_env_.getSize().y()) / (double)quant_size_player_y_;
    int player_y_quantized = (int)(player_y / player_y_step);

    if (player_y_quantized == quant_size_player_y_)
    {
        player_y_quantized--;
    }

    // ********* Quantisation Ball Coordinates ****************

    player_x = state[0];
    player_y = state[1];
    double ball_x = state[2];
    double ball_y = state[3];
    // ball_x -= hax_env_.getSize().x(); // now x is in the range [0, 2*size_x]
    // ball_y -= hax_env_.getSize().y(); // now y is in the range [0, 2*size_y]

    // return when ball is in the back of the agent
    if (abs(player_y - ball_y) < (hax_env_.getRadiusBall() + hax_env_.getRadiusPlayer()))
    {
        if (player_x - ball_x > 0)
        {
            return quant_size_player_x_ * quant_size_player_y_ * quant_size_angle_ball;
        }
    }

    double ball_relative_x = ball_x - player_x;
    double ball_relative_y = ball_y - player_y;

    double angle = atan2(ball_relative_x, ball_relative_y);
    // angle is between -pi and pi. 0 is top. negative is left side, positive is right side.

    int angle_quantized = 0;

    // Angles: 45, 23, 18, 8, 18, 23, 45

    if (angle < 0)
    {
        angle_quantized = 0;
    }
    else if (angle < (45 * M_PI / 180.0))
    {
        angle_quantized = 1;
    }
    else if (angle < (68 * M_PI / 180.0))
    {
        angle_quantized = 2;
    }
    else if (angle < (86 * M_PI / 180.0))
    {
        angle_quantized = 3;
    }
    else if (angle < (94 * M_PI / 180.0))
    {
        angle_quantized = 4;
    }
    else if (angle < (112 * M_PI / 180.0))
    {
        angle_quantized = 5;
    }
    else if (angle < (135 * M_PI / 180.0))
    {
        angle_quantized = 6;
    }
    else if (angle < (180 * M_PI / 180.0))
    {
        angle_quantized = 7;
    }

    // std::cout << "Angle Quantized: " << angle_quantized << "Angle is: " << angle << std::endl;

    int index = 0;
    index += (quant_size_angle_ball * quant_size_player_y_) * player_x_quantized;
    index += (quant_size_angle_ball)*player_y_quantized;
    index += angle_quantized;

    return index;
}

int QuantGridPlayerAngleBall::getStateSize() const
{
    return quant_size_player_x_ * quant_size_player_y_ * quant_size_angle_ball + 1;
}

std::string QuantGridPlayerAngleBall::getType() const
{
    return "QuantGridPlayerAngleBall";
}

std::vector<double> QuantGridPlayerAngleBall::getConfiguration() const
{
    std::vector<double> vec;
    vec.push_back(quant_size_player_x_);
    vec.push_back(quant_size_player_y_);
    return vec;
}