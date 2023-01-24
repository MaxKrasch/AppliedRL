#include "RewardGoal.hpp"
#include "QuantUtils.hpp"


RewardGoal::RewardGoal(int cases,int reward_agent_goal, int reward_opponent_goal) : BaseReward(cases)
{
    this->cases = cases;
    this->reward_agent_goal = reward_agent_goal;
    this->reward_opponent_goal = reward_opponent_goal;
}

double RewardGoal::get_reward(const Eigen::Ref<const Eigen::VectorXd> &o, int a, const Eigen::Ref<const Eigen::VectorXd> &o_prime)
{
    double reward = 0;
    int i = 0;

    double ballx = o(2);
    double bally = o(3);
    double ballx_next = o_prime(2);
    double bally_next = o_prime(3);
    double agentx = o(0);
    double agenty = o(1);
    double agentx_next = o_prime(0);
    double agenty_next = o_prime(1);
    Eigen::Vector2d vecP = {3.5,0.0}; 

    // Distance based reward to come close to the ball (reward function 5)
    double d0 = pow(ballx - agentx_next,2);
    double d1 = pow(bally - agenty_next,2);
    double d_prime = (d0 + d1);
    double d2 = pow(ballx - agentx,2);
    double d3 = pow(bally - agenty,2);
    double d_curr = (d2 + d3);

    switch (cases)
    {
    case 1:
    {
        // Reward 100 for scoring a goal
        if(agent_goals < haxball->getAgentGoals()){
            reward += reward_agent_goal;
        }

        // Reward -100 for a opponent goal
        if(opponent_goals < haxball->getOpponentGoals()){
            reward -= reward_opponent_goal;
        }

        // Distance based reward to come close to the ball (reward function 5)
        if(d_curr > d_prime){
            reward += 1;
        }
        else if(d_curr<=d_prime){
            reward += -1;
        }
    break;
    }
    case 2:{
        if(agent_goals < haxball->getAgentGoals()){
            reward += reward_agent_goal;
        }
        if(opponent_goals < haxball->getOpponentGoals()){
            reward -= reward_opponent_goal;
        }

        if(d_curr > d_prime){
            reward += 1;
        }
        else if(d_curr<=d_prime){
            reward += -1;
        }

        // Reward 1 for reduced distance between ball and goal
        double d_p0 = pow(vecP(0) - ballx_next,2);
        double d_p1 = pow(vecP(1) - bally_next,2);
        double d_p_prime = (d_p0 + d_p1);
        double d_p2 = pow(vecP(0) - ballx,2);
        double d_p3 = pow(vecP(1) - bally,2);
        double d_p_curr = (d_p2 + d_p3);
        if(d_p_curr > d_p_prime){
            reward += 0.5;
        }
        else if(d_p_curr<=d_p_prime){
            reward -= -0.5;
        }
        
        if (d_p_curr <= 0.05 + d_p_prime){
            i += 1;     
        }
        if (i >= 20){
            std::cout << "print i" << i;     
            reward -= 10000000;
            i = 0;
        }
    break;

        // Reward 1 for the same angle area of ball and agent (see figure 2)
        // Reward -1 for deviation of angle area of ball and agent
        // Reward 1 for reduced distance between ball and goal
        // Reward 10 when the current ball trajectory goes through the goal
    }
    case 3:{
        if(haxball->ballWasInRightGoal() == true){
            reward += reward_agent_goal;
        }
        if(haxball->ballWasInLeftGoal() == true){
            reward -= reward_opponent_goal;
        }
        
        double anglerad_agent = getAngleFromGoal_rad(agentx, agenty);
        double anglerad_ball = getAngleFromGoal_rad(ballx, bally);
        int num_angles = 11;
        int angle_agent = getScalarQuantisation(anglerad_agent, 0, M_PI, num_angles);
        int angle_ball = getScalarQuantisation(anglerad_ball, 0, M_PI, num_angles);

        /**if (angle_agent == angle_ball){
            reward += 10;
        }
        else if (abs(angle_agent-angle_ball) >= 2){
            reward -= 10;
        }**/
        if(d_curr > d_prime){
            reward += 1;
        }
        else if(d_curr<=d_prime){
            reward -= 1;
        }
        /**double dgoalagentx = pow(vecP(0) - agentx,2);
        double dgoalagenty = pow(vecP(1) - agenty,2);
        double dgoalagentcurr = (dgoalagentx + dgoalagenty);
        if(dgoalagentcurr <= 2.5){
            reward -= 10000;
        }**/

        double dgoalballx = pow(vecP(0) - ballx,2);
        double dgoalbally = pow(vecP(1) - bally,2);
        double dgoalballcurr = (dgoalballx + dgoalbally);
        double ballvelx = o(4);
        double ballvely = o(5);
        double ballvel = ballvelx + ballvely;
        if((dgoalballcurr <= 3) && (ballvel >= 0.03)){
            if (angle_agent == angle_ball){
            reward -= 100;
        }
            else if (abs(angle_agent-angle_ball) >= 1){
            reward += 100;
        }
        }

    break;
    }
    agent_goals = haxball->getAgentGoals();
    opponent_goals = haxball->getOpponentGoals();
}
    return reward;
}

std::string RewardGoal::getType() const
{
    std::string type = "RewardGoal";
    return type;
}

std::vector<double> RewardGoal::getConfiguration() const
{
    std::vector<double> vec;
    vec.push_back(cases);
    return vec;
}

void RewardGoal::setSim(HaxBall *sim)
{
    haxball = sim;
}
