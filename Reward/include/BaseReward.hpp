#ifndef _BaseReward_H_
#define _BaseReward_H_
#include "Eigen/Dense"
#include "HaxBall.h"
class BaseReward{

public:
    /**
     * @brief Construct a new Base Reward object
     * 
     */
    BaseReward();
    /**
     * @brief Construct a new Base Reward object
     * 
     * @param cases 
     */
    BaseReward(int cases);
    /**
     * @brief reward variable
     * 
     */
    double r;    
    /**
     * @brief Get the reward object
     * 
     * @param o 
     * @param a 
     * @param o_prime 
     * @return double 
     */
    virtual double get_reward(const Eigen::Ref<const Eigen::VectorXd>& o,
                int a,
                const Eigen::Ref<const Eigen::VectorXd>& o_prime)=0;

    virtual std::string getType() const=0;

    virtual std::vector<double> getConfiguration() const=0;

    virtual void setSim(HaxBall* sim)=0;


    /**
     * @brief cases variable
     * 
     */
    int cases;

    HaxBall* haxball;
};

#endif //_BaseReward_H_