#ifndef _QUANT_GRID_PLAYER_H_
#define _QUANT_GRID_PLAYER_H_


#include "BaseAgent.h"
#include "HaxBall.h"

#include "Eigen/Dense"

#include "BaseQuantisation.hpp"

class QuantGridPlayer : public BaseQuantisation{

    public:

        QuantGridPlayer(int quant_size_x, int quant_size_y);

        int getQtableStateIndex(const Eigen::Ref<const Eigen::VectorXd>& state) const;

        int getStateSize() const;

        std::string getType() const;

        std::vector<double> getConfiguration() const;


    private:
        const int quant_size_x_;
        const int quant_size_y_;

        const HaxBall hax_env_;
};


#endif // _QUANT_GRID_PLAYER_H_