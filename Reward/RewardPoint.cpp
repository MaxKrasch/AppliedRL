#include "RewardPoint.hpp"
#include "BaseReward.hpp"
#include <iostream>


/*rewardPoint::rewardPoint(int cases_, Eigen::Vector2d vecP_){
    int cases = cases_;
    Eigen::Vector2d vecP = vecP_;
};*/
RewardPoint::RewardPoint(int cases, Eigen::Vector2d vecP):BaseReward(cases){
    this->cases=cases;
    this->vecP = vecP;
}


double RewardPoint::get_reward(const Eigen::Ref<const Eigen::VectorXd>& o, int a,const Eigen::Ref<const Eigen::VectorXd>& o_prime)
{
    // field goes from left upper corner:(-4,-2); to right lower corner:(4,2);
    switch (cases)
    {
        /* Norm distance, negative reward*/
    case 1:
    {
        double r0 = pow(vecP(0) - o_prime(0), 2);
        double r1 = pow(vecP(1) - o_prime(1), 2);
        r = -(r0 + r1);
        return r;
        break;
    }
        /*Piecewise linear (always pos)*/
    case 2:
    {
        if ((vecP(0) / 2 < o(0)) && (vecP(1) / 2 < o(1)) && ((3 * vecP(1)) / 2 > o(1)) && ((3 * vecP(0)) / 2 > o(0)))
        {
            Eigen::VectorXd r1 = o_prime.head(2) - vecP;
            Eigen::VectorXd r2 = r1.cwiseAbs();
            Eigen::VectorXd r0 = vecP / 2 - r2;
            return r = r0.norm();
        }
        else
        {
            return r = 0.0;
        }
        break;
    }
    case 3:
    {
        if ((abs(vecP(0) - 2) <= o(0)) && (abs(vecP(1) - 2) <= o(1)))
        {
            return r = 5.0;
        }
        break;
    }
    // reward in circle shape
    case 4:
    {
        double d0 = pow(vecP(0) - o_prime(0),2);
        double d1 = pow(vecP(1) - o_prime(1),2);
        double d_prime = (d0 + d1);
        double d2 = pow(vecP(0) - o(0),2);
        double d3 = pow(vecP(1) - o(1),2);
        double d_curr = (d2 + d3);

        //std::cout << "current" << d_curr;
        //std::cout << "next" << d_prime;

        if((abs(d_curr-d_prime) == 0) && (d_prime > 0.1) && (d_curr > 0.1)){
            return r = -2;
        }
        else if ((abs(d_curr-d_prime) == 0) && (d_prime <= 0.1) && (d_curr <= 0.1))
        {
            return r = 10;
        }
        else if ((pow(o_prime(0), 2) + pow(o_prime(1), 2)) <= (0.5 + abs(vecP(0)) + abs(vecP(1))))
        {
            return r = 3.0;
        }
    
        else if ((pow(o_prime(0), 2) + pow(o_prime(1), 2)) <= (2 + abs(vecP(0)) + abs(vecP(1))))
        {
            return r = 2.0;
        }
        else if ((pow(o_prime(0), 2) + pow(o_prime(1), 2)) <= (5 + abs(vecP(0)) + abs(vecP(1))))
        {
            return r = 1.0;
        }
        else
        {
            return r = -1;
        }
        break;
        // reward around circle of r=3
    }
    case 5:
    {
        if ((pow(o(0), 2) + pow(o(1), 2)) <= (1 + abs(vecP(0)) + abs(vecP(1))))
        {
            r = 1 / pow(o(0) - vecP(0), 2) + pow(o(1) - vecP(1), 2);
            return r * 0.01;
        }
        else if ((pow(o(0), 2) + pow(o(1), 2)) < (5 + abs(vecP(0)) + abs(vecP(1)))){
            r = 1 / pow(o(0) - vecP(0), 2) + pow(o(1) - vecP(1), 2);
            return r * 0.01;
        }
        else
        {
            return r = 0.0;
        }
        break;
    }
    case 6:
    {
        double d0 = pow(vecP(0) - o_prime(0),2);
        double d1 = pow(vecP(1) - o_prime(1),2);
        double d_prime = (d0 + d1);
        double d2 = pow(vecP(0) - o(0),2);
        double d3 = pow(vecP(1) - o(1),2);
        double d_curr = (d2 + d3);
        if(d_curr > d_prime){
            return r = 1;
        }
        else if(d_curr<=d_prime){
            return r= -1;
        }
        //else if((d_curr==d_prime) && (d_prime > 0.1) && (d_curr > 0.1)){
            //return r= -1;
        //}
         
        break;
    }
    case 7:
    {
        double dist_sqaured = pow(vecP(0) - o_prime(0), 2) + pow(vecP(1) - o_prime(1), 2);
        double dist = sqrt(dist_sqaured);
        if(dist<0.2){
            r=1;
            // std::cout << "Perfekt" << "\n";
        }
        else{
            r=0;
        }
        return r;
        break;
    }
}
return r= 0.0;
}

std::string RewardPoint::getType() const{
    std::string type = "RewardPoint";
    return type;
}


std::vector<double> RewardPoint::getConfiguration() const{
    std::vector<double> vec;
    vec.push_back(cases);
    vec.push_back(vecP[0]);
    vec.push_back(vecP[1]);
    return vec;
}


void RewardPoint::setSim(HaxBall* sim){
    haxball = sim;
}

