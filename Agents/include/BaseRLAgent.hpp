#ifndef _BASE_RL_AGENT_H_
#define _BASE_RL_AGENT_H_


#include "BaseAgent.h"
#include "HaxBall.h"

#include "BaseReward.hpp"
#include "BaseQuantisation.hpp"

#include "greediness.hpp"

#include "Eigen/Dense"

// Standard Library
#include <string.h>
#include <fstream>
#include <random>
#include <filesystem>

namespace fs = std::filesystem;

/**
 * @brief 
 * 
 */
class BaseRLAgent : public BaseAgent
{
public:

  /**
   * @brief Unused function, needed for inheritance from BaseAgent
   * 
   * @param state the integer describing a state
   * @return a discrete action suitable for executing in the world
   */
  virtual int policy(int state) const=0;


  /**
   * @brief Policy of the agent
   * 
   * @param obs Current state of HaxBall
   * @return a discrete action suitable for executing in the world 
   */
  virtual int policy(const Eigen::Ref<const Eigen::VectorXd>& obs) const=0;


  /**
   * @brief Loads the agent from folder @agent_folder.
   * 
   * @param agent_folder Folder that contains all files of the agent.
   */
  virtual void load(std::filesystem::path agent_folder)=0;


  /**
   * @brief Saves the agent in folder @param agent_folder
   * 
   * @param agent_folder Folder that contains all files of the agent. If the folder already exists it get overwritten.
   */
  virtual void save(fs::path agent_folder) const =0;

  /**
   * @brief Start a training process of the agent.
   * 
   * @param epochs 
   * @param alpha Learning Rate
   * @param gamma Discount Factor
   * @param epsilon probability of choosing a random action
   * @param sim  The simulation to train in. Needed so that the training environment can be changed.
   * @return double 
   */
  virtual std::vector<double> train(int epochs, double alpha, double gamma, HaxBall &sim, int trajectory_length, Greediness* greediness)=0;   // returns the accumulated reward

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
  virtual double reward(const Eigen::Ref<const Eigen::VectorXd>& state_curr,
                int a,
                const Eigen::Ref<const Eigen::VectorXd>& state_next) const=0;


  /**
   * @brief Get a string containing the name of the agent type.
   * 
   * @return std::string agent_type
   */
  virtual std::string getType() const =0;


  /**
   * @brief Prints info about the agent and its attributes
   * 
   */
  virtual void printInfo() const =0;

  BaseReward* reward_class_;
  BaseQuantisation* quant_class_;
  Eigen::VectorXd action_vector;
  
};


#endif  // _BASE_RL_AGENT_H_