#ifndef _QUANT_GRID_PLAYER_ANGLE_BALL_H_
#define _QUANT_GRID_PLAYER_ANGLE_BALL_H_


#include "BaseAgent.h"
#include "HaxBall.h"

#include "Eigen/Dense"

#include "BaseQuantisation.hpp"
#include <iostream>


class QuantGridPlayerAngleBall : public BaseQuantisation{

    public:

        QuantGridPlayerAngleBall(int quant_size_x, int quant_size_y);

        int getQtableStateIndex(const Eigen::Ref<const Eigen::VectorXd>& state) const;

        int getStateSize() const;

        std::string getType() const;

        std::vector<double> getConfiguration() const;


    private:
        const int quant_size_player_x_;
        const int quant_size_player_y_;

        const int quant_size_angle_ball = 8;

        const HaxBall hax_env_;
};


#endif // _QUANT_GRID_PLAYER_ANGLE_BALL_H_