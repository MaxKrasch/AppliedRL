#ifndef _Q_AGENT_2_H_
#define _Q_AGENT_2_H_


#include "BaseRLAgent.hpp"
#include "BaseReward.hpp"
#include "greediness.hpp"
#include <vector>

#include <iostream>
#include <random>
#include <algorithm>

#include "BaseQuantisation.hpp"

#include <thread>






class Q_Agent_2 : public BaseRLAgent
{
public:

  Q_Agent_2(const Eigen::Ref<const Eigen::VectorXd>& action_vector_, BaseQuantisation* quant_class, BaseReward* reward_class);

  ~Q_Agent_2(){};

  /**
   * @brief Unused function, needed for inheritance from BaseAgent
   * 
   * @param state the integer describing a state
   * @return a discrete action suitable for executing in the world
   */
  int policy(int state) const;


  /**
   * @brief Policy of the agent
   * 
   * @param obs Current state of HaxBall
   * @return a discrete action suitable for executing in the world 
   */
  int policy(const Eigen::Ref<const Eigen::VectorXd>& obs) const;


  /**
   * @brief Loads the agent from folder @agent_folder.
   * 
   * @param agent_folder Folder that contains all files of the agent.
   */
  void load(fs::path agent_folder);


  /**
   * @brief Saves the agent in folder @param agent_folder
   * 
   * @param agent_folder Folder that contains all files of the agent. If the folder already exists it get overwritten.
   */
  void save(fs::path agent_folder) const;

  /**
   * @brief Start a training process of the agent.
   * 
   * @param epochs 
   * @param alpha Learning Rate
   * @param gamma Discount Factor
   * @param epsilon probability of choosing a random action
   * @param sim The simulation to train in. Needed so that the training environment can be changed.
   * @return double 
   */
  std::vector<double> train(int epochs, double alpha, double gamma, HaxBall &sim, int trajectory_length, Greediness* greediness); 


  /**
   * @brief Returns the column index in the Q_Matrix representing the state @param state
   * 
   * @param state HaxBall State
   * @return int State index in the Q_Matrix
   */
  int getQtableStateIndex(const Eigen::Ref<const Eigen::VectorXd>& state) const;


  ///
  /// \brief reward Your one step reward function
  /// \param state_curr Current (continuous) state
  /// \param a Executed action
  /// \param state_next Successor state (also continuous)
  /// \return the one step reward r(s,a,s')
  ///
  /// The reward function, use only these three inputs to avoid defining the reward with quantities the agent cannot observe.
  /// Of course your agent can store an extra instance of HaxBall to access static information like the goal area.
  ///
  double reward(const Eigen::Ref<const Eigen::VectorXd>& state_curr,
                int a,
                const Eigen::Ref<const Eigen::VectorXd>& state_next) const;

  /**
   * @brief Get a string containing the name of the agent type.
   * 
   * @return std::string agent_type
   */
  std::string getType() const;


  Eigen::VectorXd getActionVector() const;


  /**
   * @brief Prints info about the agent and its attributes
   * 
   */
  void printInfo() const;

  double get_multi_step_reward(std::vector<double> rewards, int step_number, double gamma_mulit);
  //BaseReward* reward_class_;
  //BaseQuantisation* quant_class_;
  //Eigen::VectorXd action_vector;


private:
  /**
   * @brief Get the index of the Best Action from the QTable corresponding to the stateIndex. If multiple values are the same, a random action of the best ones is returned.
   * 
   * @return int 
   */
  int getBestActionIndex(int stateIndex) const;


  /**
   * @brief Returns a random index out of the possible action indexes
   * 
   * @return random action index 
   */
  int getRandomActionIndex() const;



  Eigen::MatrixXd Q_table;


  HaxBall hax_env; // for getting static infos like filed size.

};


#endif  // _Q_AGENT_2_H_
