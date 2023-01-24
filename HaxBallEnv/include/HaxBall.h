#ifndef _HAXBALL_H_
#define _HAXBALL_H_

#include <random>

#include <QObject>
#include <QRectF>

#include "Eigen/Dense"

///
/// \brief The HaxBall class with the differential equations
///
/// This class contains the game HaxBall and the simulation code.
/// You can create as many instances as you like and use them for training.
///
/// This class uses QT code, because QRectF and others are quite handy.
/// Also the signal slot mechanism is available, let me know what you need and I add it here.
///
/// Important: The game uses Qt's coordinate system
///   +------> x
///   |
///   |
///   V
///   y
///
/// The environment is inspired and partially based on a student project from ARL 2020.
/// For details ask Martin Gottwald.
///
class HaxBall: public QObject
{
  Q_OBJECT

public:
  ///
  /// \brief HaxBall Creates a new HaxBall environment
  /// \param has_opponent Enables or disables the opponent
  /// \param parent the parent of this QObject (if any)
  ///
  /// Contains only the initialization of all values and creates the starting state by resetting the environment
  ///
  HaxBall(bool has_opponent = true, QObject* parent = 0);
  virtual ~HaxBall();

  ///
  /// \brief getTimeDelta
  /// \return the elapsing time between two steps
  ///
  qreal getTimeDelta() const;

  ///
  /// \brief getMaxSpeedBall
  /// \return the maximum speed of the ball
  ///
  /// The maximum speed is the same for x and y axis.
  /// So the actual max speed is bigger along the diagonals ...
  ///
  qreal getMaxSpeedBall() const;

  ///
  /// \brief getMaxSpeedPlayer
  /// \return the maximum speed of the player
  ///
  /// The maximum speed is the same for x and y axis.
  /// So the actual max speed is bigger along the diagonals ...
  ///
  qreal getMaxSpeedPlayer() const;

  ///
  /// \brief getSize
  /// \return the size of the playing ground
  ///
  QRectF getSize() const;

  ///
  /// \brief getGoalLeft
  /// \return the rectangle corresponding to the left (player) goal
  ///
  QRectF getGoalLeft() const;

  ///
  /// \brief getGoalLeft
  /// \return the rectangle corresponding to the right (opponent) goal
  ///
  QRectF getGoalRight() const;

  ///
  /// \brief getRadiusBall
  /// \return the radius of the ball (not the diameter)
  ///
  qreal getRadiusBall() const;

  ///
  /// \brief getRadiusPlayer
  /// \return the radius of the player (not the diameter)
  ///
  qreal getRadiusPlayer() const;

  ///
  /// \brief getPlayerPos
  /// \return the position of the player as QPointF
  ///
  /// For training it should be better to get once the Eigen::VectorXd and process the state in the agent.
  /// This getter is for plotting.
  ///
  QPointF getPlayerPos() const;

  ///
  /// \brief getBallPos
  /// \return the position of the ball as QPointF
  ///
  /// For training it should be better to get once the Eigen::VectorXd and process the state in the agent.
  /// This getter is for plotting.
  ///
  QPointF getBallPos() const;

  ///
  /// \brief getOpponentPos
  /// \return the position of the opponent as QPointF
  ///
  /// This getter is for plotting.
  ///
  QPointF getOpponentPos() const;

  ///
  /// \brief ballInLeftGoal
  /// \return true, if the ball hit the left goal area during the execution of a step. Otherwise false.
  ///
  bool ballWasInLeftGoal() const;

  ///
  /// \brief ballInRightGoal
  /// \return true, if the ball hit the right goal area during the execution of a step. Otherwise false.
  ///
  bool ballWasInRightGoal() const;

  ///
  /// \brief getState
  /// \param obs An eigen reference to a column vector, it receives a copy of the state vector
  ///
  /// Important:
  /// - References cannot be resized
  /// - it is up to you to provide a vector with correct size (currently getNO() x 1)
  ///
  void getState(Eigen::Ref<Eigen::VectorXd> obs) const;
  
  ///
  /// \brief setState
  /// \param obs A constant eigen reference to a column vector describing the new environment state
  ///
  ///
  ///
  void setState(const Eigen::Ref<const Eigen::VectorXd>& obs);

  ///
  /// \brief step Executes the action in the world
  /// \param action the i-th action to execute
  ///
  /// This function should be fail safe, once it returns the internal state is updated.
  /// Of course the step is not thread safe!
  ///
  void step(int action);

  ///
  /// \brief getNO
  /// \return the (hardcoded) number of obervations
  ///
  int getNO() const { return 6; }

  ///
  /// \brief getNA
  /// \return the (hardcoded) number of discrete actions
  ///
  int getNA() const { return 18; }

  ///
  /// \brief reset Resets the state to some random value
  ///
  /// The Player is positioned somewhere on the play ground.
  /// The Ball as well (maybe inside the player ...)
  /// The Ball position is somewhere between the max values
  ///
  /// All distributions are uniform.
  ///
  void reset();

  ///
  /// \brief hasOpponent
  /// \return true, if an opponent is present
  ///
  /// Used for plotting to toggle the rendering of an opponent
  ///
  bool hasOpponent() const;

  /// \brief Returns the number of goals of your agent since the last reset
  /// \return The number of agent goals since last reset
  ///
  /// Counts the number of goal since the last reset, do not use this value in your
  /// reward function as the counter depends on states from the past.
  ///
  int getAgentGoals() const;

  /// \brief Returns the number of goals of the opponent since the last reset
  /// \return The number of opponent goals since last reset
  ///
  /// Counts the number of goal since the last reset, do not use this value in your
  /// reward function as the counter depends on states from the past.
  ///
  int getOpponentGoals() const;

private:

  ///
  /// \brief resets other variables outside of the state vector
  ///
  /// This function is responsible to reset additional variables of the environment
  /// such as the was-in-goal indicators.
  ///
  void resetOthers();

  ///
  /// \brief action_map Maps discrete actions to a continuous quantity
  /// \param action discrete action
  /// \param action_mapped corresponding continuous action as eigen reference
  ///
  /// Use this function to understand which action is doing what
  ///
  void action_map(const int action, Eigen::Ref<Eigen::Vector3d> action_mapped) const;

  ///
  /// \brief collision Handles the collision between two particles
  /// \param p1 Position of Particle 1
  /// \param v1 Velocity of Particle 1
  /// \param r1 Radius of Particle 1
  /// \param p2 Position of Particle 2
  /// \param v2 Velocity of Particle 2
  /// \param r2 Radius of Particle 2
  ///
  /// Particle 1 is infinte heavy.
  /// Particle 2 has no mass, in bounces perfectly at the surface of particle 1.
  ///
  /// The method corrects penetrations between particles by projecting Particle 2 back to the surface
  /// The vectors remain unchanged if there is no collision (distance > sum of radii)
  ///
  void collision(Eigen::Ref<Eigen::Vector2d> p1, Eigen::Ref<Eigen::Vector2d> v1, double r1,
                 Eigen::Ref<Eigen::Vector2d> p2, Eigen::Ref<Eigen::Vector2d> v2, double r2) const;

  ///
  /// \brief random_number Produces a random number in given interval
  /// \param low lower limit of interval
  /// \param high higher limit of interval
  /// \return the random number
  ///
  /// Attention: uses a dirty hack instead of the proper std::c++ random machanism with that distribution parameter info thing.
  ///
  double random_number(double low, double high);

private:

  // Stuff for the random number generator
  std::random_device m_random_device;
  std::mt19937 m_random_engine;
  std::uniform_real_distribution<> m_uniform_dist;

  // Variables to describe the simulation
  QRectF m_size, m_goal_left, m_goal_right;
  qreal m_radius_player, m_radius_ball;
  qreal m_dt, m_max_speed_player, m_max_speed_ball, m_friction, m_shoot_dist;

  // Variables to make the work with an agent easier
  bool m_wasInLeftGoal, m_wasInRightGoal;

  // A true column vector with getNO() components
  Eigen::VectorXd m_state;

  // Change its value in the constructor to switch between easy mode or with oppnent
  const bool m_has_opponent;

  // The position of the opponent, it is defined directly from the state vector and can stay outside of m_state
  // The variable must be a member for rendering.
  Eigen::Vector2d m_position_opponent;

  // The velocity of the opponent, currently it is always zero and just required to call the collision function
  Eigen::Vector2d m_velocity_opponent;

  // The right goal center as Eigen::Vector for easy computations
  Eigen::Vector2d m_goal_right_center;

  // Number of goals
  int m_num_agent_goals, m_num_opponent_goals;
};

#endif // _HAXBALL_H_
