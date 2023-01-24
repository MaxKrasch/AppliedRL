#ifndef _RewardGoal_H_
#define _RewardGoal_H_


#include "Eigen/Dense"
#include "BaseReward.hpp"

#include <iostream>
#include "QuantUtils.hpp"



class RewardGoal: public BaseReward{

public:
    RewardGoal(int cases,int reward_agent_goal, int reward_opponent_goal);
    double get_reward(const Eigen::Ref<const Eigen::VectorXd>& o, int a,const Eigen::Ref<const Eigen::VectorXd>& o_prime);

    std::string getType() const;
    std::vector<double> getConfiguration() const;

    void setSim(HaxBall* sim);

    
    Eigen::Vector2d ball_velocity_old;
    Eigen::Vector2d ball_velocity_new;

    int reward_opponent_goal;
    int reward_agent_goal;

    int agent_goals;
    int opponent_goals;
};

#endif //_RewardGoal_H_