#ifndef _RewardBall_H_
#define _RewardBall_H_


#include "Eigen/Dense"
#include "BaseReward.hpp"

#define REWARD_BALL_VELOCITY_INCREASE 7


class RewardBall: public BaseReward{

public:
    RewardBall(int cases);
    double get_reward(const Eigen::Ref<const Eigen::VectorXd>& o, int a,const Eigen::Ref<const Eigen::VectorXd>& o_prime);

    std::string getType() const;
    std::vector<double> getConfiguration() const;

    void setSim(HaxBall* sim);

    
    Eigen::Vector2d ball_velocity_old;
    Eigen::Vector2d ball_velocity_new;
};

#endif //_RewardBall_H_