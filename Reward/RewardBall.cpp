#include "RewardBall.hpp"
#include "BaseReward.hpp"


RewardBall::RewardBall(int cases):BaseReward(cases){
    this->cases=cases;
    ball_velocity_old << 0.0,0.0;
    ball_velocity_new << 0.0,0.0;
}


double RewardBall::get_reward(const Eigen::Ref<const Eigen::VectorXd>& o, int a,const Eigen::Ref<const Eigen::VectorXd>& o_prime)
{
    //ball states
    double ballx = o(2);
    double bally = o(3);

    ball_velocity_old = ball_velocity_new;

    ball_velocity_new[0] = o_prime[2] - o[2];
    ball_velocity_new[1] = o_prime[3] - o[3];


    switch (cases)
    {
        /* Norm distance, negative reward*/
    case 1:
    {
        double r0 = pow(ballx - o_prime(0), 2);
        double r1 = pow(bally - o_prime(1), 2);
        r = -(r0 + r1);
        return r;
        break;
    }
        /*Piecewise linear (always pos)*/
    case 3:
    {
        if ((abs(ballx - 2) <= o(0)) && (abs(bally - 2) <= o(1)))
        {
            return r = 5.0;
        }
        break;
    }
    // reward in circle shape
    case 4:
    {
        if ((pow(o_prime(0), 2) + pow(o_prime(1), 2)) <= (0.05 + abs(ballx) + abs(bally)))
        {
            return r = 10;
        }
        else if ((pow(o(0), 2) + pow(o(1), 2)) <= (1 + abs(ballx) + abs(bally)))
        {
            return r = 5.0;
        }
        else if ((pow(o(0), 2) + pow(o(1), 2)) <= (5 + abs(ballx) + abs(bally)))
        {
            return r = 3.0;
        }

        else
        {
            return r = 0;
        }
        break;
    }
        // reward around circle of r=3
    case 5:
    {
        if ((pow(o(0), 2) + pow(o(1), 2)) <= (0.5 + abs(ballx) + abs(bally)))
        {
            r = 1 / pow(o(0) - ballx, 2) + pow(o(1) - bally, 2);
            return r * 0.01;
        }
        else if ((pow(o(0), 2) + pow(o(1), 2)) < (6 + abs(ballx) + abs(bally))){
            r = 1 / pow(o(0) - ballx, 2) + pow(o(1) - bally, 2);
            return r * 0.03;
        }
        else
        {
            return r = 0.0;
        }
        break;
    }
    case 6:
    {
        double d0 = pow(ballx - o_prime(0),2);
        double d1 = pow(bally - o_prime(1),2);
        double d_prime = (d0 + d1);
        double d2 = pow(ballx - o(0),2);
        double d3 = pow(bally - o(1),2);
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
    case REWARD_BALL_VELOCITY_INCREASE:
    {
        double velocity_change = ball_velocity_new.norm() - ball_velocity_old.norm();
        if(a == 0){
            return -10;
        }

        if(abs(velocity_change) > 0.05){
            return 1000;
        }else{
            return 0;
        }
    }
    }
    
    return r = 1.0;
}

std::string RewardBall::getType() const{
    std::string type = "RewardBall";
    return type;
}

std::vector<double> RewardBall::getConfiguration() const{
    std::vector<double> vec;
    vec.push_back(cases);
    return vec;
}

void RewardBall::setSim(HaxBall* sim){
    haxball = sim;
}


