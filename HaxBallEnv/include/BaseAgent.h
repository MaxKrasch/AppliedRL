#ifndef _BASEAGENT_H_
#define _BASEAGENT_H_

#include "Eigen/Dense"

///
/// \brief The BaseAgent class
///
/// Provides an interface for agents. Your agent must subclass the BaseAgent and implement the policy functions
///
///
class BaseAgent
{
public:

  /// Nothing happens here
  BaseAgent();
  virtual ~BaseAgent();

  ///
  /// \brief policy
  /// \param state the integer describing a state
  /// \return a discrete action suitable for executing in the world
  ///
  /// This version is mostly for you to work with.
  ///
  virtual int policy(int state) const = 0;

  ///
  /// \brief policy
  /// \param obs the continuous state from the environment
  /// \return a discrete action suitable for executing in the world
  ///
  /// This version gets used by the HaxBallGui to convert the continuous state into an action
  ///
  /// \overload
  virtual int policy(const Eigen::Ref<const Eigen::VectorXd>& obs) const = 0;

private:

};

#endif
