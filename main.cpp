#include <iostream>

#include <QApplication>

#include "HaxBall.h"
#include "HaxBallGui.h"

#include "RandomAgent.h"

#include "trainer.hpp"
#include "BaseRLAgent.hpp"
#include "Q_Agent.hpp"
#include "Q_Agent_2.hpp"

#include "RewardPoint.hpp"
#include <thread>

#include <stdlib.h>     /* srand, rand */
// #define STATIC_GREEDY = 1;
// #define CONSECUTIVE_ACTIONS_GREEDY = 2;
// #define TIME_DECAY_GREEDY = 3;

void render(BaseAgent& agent, int argc, char** argv)
{
  QApplication app(argc, argv);
  HaxBallGui gui(agent);
  gui.show();
  gui.playGame(10.0, 5e4);
  app.exec();
}

void print_reward(std::shared_ptr<HaxBall> &haxball, BaseRLAgent *agent)
{
    Eigen::Vector<double, 6> old_state;
    Eigen::Vector<double, 6> new_state;

    for (int i = 0; i < 10000; i++)
    {

        haxball->getState(old_state);

        // int random_action = rand() %5; // random number between 0 and 4;
        // haxball->step(random_action);

        std::this_thread::sleep_for(std::chrono::milliseconds(40));

        haxball->getState(new_state);

        std::cout << "Reward: " << agent->reward(old_state, 1, new_state);
        std::cout << " Player-X: " << haxball->getPlayerPos().x() << " Player-Y: " << haxball->getPlayerPos().y() << "\n";
    }
    std::cout << "Thread is finished\n";
}


int main(int argc, char** argv)
{
  std::cout << "Hello Group Group-F!" << std::endl;

  if(argc == 2){
    std::string agent_name = argv[1];
    Trainer trainer2;
    trainer2.loadAgentFromFile(agent_name);
    argc=1;
    trainer2.render(argc, argv, false, print_reward);
    return 0;
  }else if(argc == 3){
    std::string agent_name = argv[1];
    Trainer trainer2;
    trainer2.loadAgentFromFile(agent_name);
    argc=1;
    trainer2.render(argc, argv, true, print_reward);
    return 0;
  }

  // *************** Action Vectors *****************************

  // all actions without shooting
  Eigen::VectorXd action_vector_large(9);
  action_vector_large << 0, 2, 4, 6, 8, 10, 12, 14, 16;
  
  // smaller action space
  Eigen::VectorXd action_vector_small(5);
  action_vector_small << 0, 2, 6, 10, 14;

//smaller action space with shooting
  Eigen::VectorXd action_vector_small_shoot(9);
  action_vector_small_shoot << 0, 1, 2, 5, 6, 9, 10, 13, 14;

   // all actions with shooting
  Eigen::VectorXd action_vector_all(17);
  action_vector_all << 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16;

  //smaller action space with shooting (shoot north east and south east and east)
  Eigen::VectorXd action_vector_small_useful(8);
  action_vector_small_useful << 0, 2, 6, 10, 13, 14, 15, 17;

    //move up, down, left, right, and move north east, south east and shooting towards goal 
  Eigen::VectorXd action_vector_useful(9);
  action_vector_useful << 2, 6, 10, 12, 13, 14, 15, 16, 17;


  // *************** Quantisation *****************************

  std::vector<Quant*> quants;

  quants.push_back(new QuantPlayerPosX(10));

  //quants.push_back(new QuantPlayerPosY(20));

  //quants.push_back(new QuantVelocity(3));

  //quants.push_back(new QuantVelocityX(3));

  //quants.push_back(new QuantVelocityY(3));

  //quants.push_back(new QuantBallPosX(3));

  //quants.push_back(new QuantBallPosY(3));

  //quants.push_back(new QuantAngleBallToPlayer(20));

  //quants.push_back(new QuantAngleBallToGoal(8));

  quants.push_back(new QuantAnglePlayerToGoal(15));
  
  //quants.push_back(new QuantBallBehindAgent());

  quants.push_back(new QuantAngleBallToPlayerComplex(15,3,true));

  std::vector<double> bin_borders{0.45, 0.55, 0.60, 2, 4};

  quants.push_back(new QuantDistanceBallToPlayer(bin_borders));

  //std::vector<double> bin_borders2{0.45, 0.55 , 0.6, 2, 4};

  //quants.push_back(new QuantDistanceBallToGoal(bin_borders2));


  Quantisation quantisation(quants);


  // *************** Reward *****************************

  //Reward function parameters; cases define the reward function type, vecP is the Point to reach 
  int cases = 1;
  int reward_agent_goal = 10000; // reward for a goal
  int reward_opponent_goal = 10000; // - reward for a goal of opponent

  //Eigen::Vector2d vecP = {0.0,0.0};  

  //RewardPoint reward(cases, vecP);
  //RewardBall reward(cases);
  RewardGoal reward(cases, reward_agent_goal, reward_opponent_goal);


  
  
  // *************** Agent *****************************

  Q_Agent qagent(action_vector_small_shoot, &quantisation, &reward);
  Trainer trainer;

  trainer.loadAgent(&qagent);

  

  int epochs = 100000; // number of trajectories, the agent is trained in the environment
  int trajectory_length = 5e4;  // number of time steps per epoch/trajectory
  double alpha = 0.001;  // learning rate, 0.001
  double gamma = 0.95;  // discount, 0.9


  // *************** Greediness *****************************
  Greediness greediness;

  double epsilon = 0.1; // probability of choosing a random action
  //greediness.initialiseConstantEpsilon(epsilon);


  double start_probability = 1;
  double end_probability = 0.000001;  // for logepsilon: after 90% of training endprobability + 0.00001 is reached
  double target_epoch = 0.9;  // desctibes number of epochs after which the endpobability is reached e.g. 0.9 ~ after 90% of all epochs
  int repeat_action = 0;
  greediness.initialiseLinearEpsilon(start_probability, end_probability, epochs, repeat_action);

  //greediness.initialiseLogEpsilonTimestep(start_probability, end_probability, epochs, repeat_action, target_epoch);

  
  // *************** Save Agent *****************************
  // Uncomment if you want to save the agent
  trainer.startDocumentation("MK_1_neu_neu");

  bool opponent = true;
  trainer.train(epochs, alpha, gamma, epsilon, trajectory_length, &greediness, opponent);


  trainer.benchmarkAgent(opponent);

  std::cout << "Training complete. Enter a character for rendering";
  std::cin.get();
  

  trainer.render(argc, argv, opponent, print_reward);

  return 0;
}

