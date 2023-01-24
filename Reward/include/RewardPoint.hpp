#ifndef _RewardPoint_H_
#define _RewardPoint_H_


#include "Eigen/Dense"
#include "BaseReward.hpp"


class RewardPoint: public BaseReward{

public:
    Eigen::Vector2d vecP;
    RewardPoint(int cases, Eigen::Vector2d vecP);
    double get_reward(const Eigen::Ref<const Eigen::VectorXd>& o, int a,const Eigen::Ref<const Eigen::VectorXd>& o_prime);

    void setSim(HaxBall* sim);

    std::string getType() const;
    std::vector<double> getConfiguration() const;
};

#endif //_RewardPoint_H_