#ifndef _QUANT_GRID_PLAYER_BALL_H_
#define _QUANT_GRID_PLAYER_BALL_H_


#include "BaseAgent.h"
#include "HaxBall.h"

#include "Eigen/Dense"

#include "BaseQuantisation.hpp"

class QuantGridPlayerBall : public BaseQuantisation{

    public:

        QuantGridPlayerBall(int quant_size_player_x, int quant_size_player_y, int quant_size_ball_x, int quant_size_ball_y);

        int getQtableStateIndex(const Eigen::Ref<const Eigen::VectorXd>& state) const;

        int getStateSize() const;

        std::string getType() const;

        std::vector<double> getConfiguration() const;

    private:
        const int quant_size_player_x_;
        const int quant_size_player_y_;
        const int quant_size_ball_x_;
        const int quant_size_ball_y_;

        const HaxBall hax_env_;
};


#endif // _QUANT_GRID_PLAYER_BALL_H_