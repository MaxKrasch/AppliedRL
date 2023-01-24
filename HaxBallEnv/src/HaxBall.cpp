#include "HaxBall.h"

#include <iostream>
#include <sstream>  // Currently only for the error message in action_map
#include <QDebug>   // For debugging

HaxBall::HaxBall(bool has_opponent, QObject* parent) : QObject(parent),
  m_random_engine(m_random_device()), m_uniform_dist(0.0, 1.0),
  m_radius_player(0.25), m_radius_ball(0.15),
  m_dt(1.0/60.0), m_max_speed_player(0.075), m_max_speed_ball(3.0 * m_max_speed_player), m_friction(0.9995),
  m_shoot_dist(0.15 + m_radius_player + m_radius_ball),
  m_wasInLeftGoal(false), m_wasInRightGoal(false),
  m_has_opponent(has_opponent), m_num_agent_goals(0), m_num_opponent_goals(0)
{
  // Constructor needs the top left corner and the bottom right one
  m_size = QRectF(-4.0, -2.0, 8.0, 4.0);

  m_goal_left = QRectF(QPointF(0.9 * m_size.left() - 0.15, 0.25 * m_size.top() - 0.0),
                       QPointF(0.9 * m_size.left() + 0.15, 0.25 * m_size.bottom() + 0.0));

  m_goal_right = QRectF(QPointF(0.9 * m_size.right() - 0.15, 0.25 * m_size.top() - 0.0),
                        QPointF(0.9 * m_size.right() + 0.15, 0.25 * m_size.bottom() + 0.0));

  m_position_opponent << 0.0, 0.0;
  m_velocity_opponent << 0.0, 0.0;
  m_goal_right_center << m_goal_right.center().x(), m_goal_right.center().y();

  m_state.resize(6);

  // For debugging you can set a specific state here
  //  m_state << m_size.left(), m_size.top(),
  //             1.0, 0.0,
  //             -m_max_speed_ball, 0.0;

  reset();
}
HaxBall::~HaxBall(){}

qreal HaxBall::getTimeDelta() const{ return m_dt; }

qreal HaxBall::getMaxSpeedBall() const{ return m_max_speed_ball; }
qreal HaxBall::getMaxSpeedPlayer() const { return m_max_speed_player; }

QRectF HaxBall::getSize() const{return m_size;}
QRectF HaxBall::getGoalLeft() const{return m_goal_left;}
QRectF HaxBall::getGoalRight() const{return m_goal_right;}

qreal HaxBall::getRadiusBall() const{return m_radius_ball; }
qreal HaxBall::getRadiusPlayer() const{return m_radius_player; }

QPointF HaxBall::getPlayerPos() const{ return QPointF(m_state(0), m_state(1)); }
QPointF HaxBall::getBallPos() const{ return QPointF(m_state(2), m_state(3)); }
QPointF HaxBall::getOpponentPos() const { return QPointF(m_position_opponent(0), m_position_opponent(1)); }

bool HaxBall::ballWasInLeftGoal() const { return m_wasInLeftGoal; }
bool HaxBall::ballWasInRightGoal() const { return m_wasInRightGoal; }

void HaxBall::getState(Eigen::Ref<Eigen::VectorXd> obs) const { obs = m_state; }
void HaxBall::setState(const Eigen::Ref<const Eigen::VectorXd>& obs)
{
  // Setting the state from the outside invalidates the remaining internal state
  // of the environment
  resetOthers();

  m_state = obs;
}

void HaxBall::step(int action)
{
  Eigen::Vector2d player_pos = m_state.segment(0, 2);
  Eigen::Vector2d ball_pos = m_state.segment(2, 2);
  Eigen::Vector2d ball_vel = m_state.segment(4, 2);

  // Parse actions and separte player velocity from shooting indicator
  Eigen::Vector3d action_mapped;
  action_map(action, action_mapped);

  Eigen::Vector2d player_vel = m_max_speed_player * action_mapped.segment(0,2);
  bool shoot = action_mapped(2) > 0.5;

  // friction is a percentage, i.e. what part of the velocity "survives"
  // Currently, there is no integration of accelerations required, e.g. wind.
  ball_vel = ball_vel * m_friction;

  // Euler integration
  ball_pos = ball_pos + m_dt * ball_vel;
  player_pos = player_pos + m_dt * player_vel;

  if(m_has_opponent)
  {
    // Definie opponent position to be between goal and ball
    m_position_opponent = player_pos - m_goal_right_center;
    m_position_opponent = m_goal_right_center + 1.0 * m_position_opponent / m_position_opponent.norm();
  }

  // handle player-ball collision, player is inf heavy mass so the ball gets all
  collision(player_pos, player_vel, m_radius_player,
            ball_pos, ball_vel, m_radius_ball);

  if(m_has_opponent)
  {
    // handle opponent-ball collision, opponent is inf heavy mass so the ball gets all
    collision(m_position_opponent, m_velocity_opponent, m_radius_player,
              ball_pos, ball_vel, m_radius_ball);

    // handle player-opponent collision, both are infinte heavy ...
    // opponent stays in place, player is projected to the outside
    collision(m_position_opponent, m_velocity_opponent, m_radius_player,
              player_pos, player_vel, m_radius_player);
  }

  // Collision with infinite heavy borders -> reflection for ball
  if( ball_pos(0) < m_size.left() or ball_pos(0) > m_size.right())
    ball_vel(0) *= -1.0;

  if( ball_pos(1) < m_size.top() or ball_pos(1) > m_size.bottom())
    ball_vel(1) *= -1.0;

  // Clip position to keep player and ball on the screen
  if (player_pos(0) < m_size.left()) player_pos(0) = m_size.left();
  if (player_pos(0) > m_size.right()) player_pos(0) = m_size.right();
  if (player_pos(1) < m_size.top()) player_pos(1) = m_size.top();
  if (player_pos(1) > m_size.bottom()) player_pos(1) = m_size.bottom();

  if (ball_pos(0) < m_size.left()) ball_pos(0) = m_size.left();
  if (ball_pos(0) > m_size.right()) ball_pos(0) = m_size.right();
  if (ball_pos(1) < m_size.top()) ball_pos(1) = m_size.top();
  if (ball_pos(1) > m_size.bottom()) ball_pos(1) = m_size.bottom();

  if (ball_vel(0) < -m_max_speed_ball) ball_vel(0) = -m_max_speed_ball;
  if (ball_vel(0) > +m_max_speed_ball) ball_vel(0) = +m_max_speed_ball;
  if (ball_vel(1) < -m_max_speed_ball) ball_vel(1) = -m_max_speed_ball;
  if (ball_vel(1) > +m_max_speed_ball) ball_vel(1) = +m_max_speed_ball;

  // Handle shooting, if ball is in range its velocity gets overwritten
  Eigen::Vector2d diff = ball_pos - player_pos;
  double distance = diff.norm();

  if (shoot and distance < m_shoot_dist and distance > 1e-5)
    ball_vel = m_max_speed_ball * diff / distance;

  // Store this indicators to be able to tell what happened during the execution of step()
  m_wasInLeftGoal = m_goal_left.contains(ball_pos(0), ball_pos(1));
  m_wasInRightGoal = m_goal_right.contains(ball_pos(0), ball_pos(1));

  if( m_wasInLeftGoal ) m_num_opponent_goals++;
  if( m_wasInRightGoal ) m_num_agent_goals++;

  // Reset ball position to center without velocity if it touches a goal area
  if( m_wasInLeftGoal or m_wasInRightGoal )
  {
    ball_pos.fill(0.0);
    ball_vel.fill(0.0);
  }

  // Store the updated state as single vector (for the agent to process)
  m_state <<
      player_pos(0), player_pos(1),
      ball_pos(0), ball_pos(1),
      ball_vel(0), ball_vel(1);

  return;
}

void HaxBall::reset()
{
  m_state(0) = random_number(m_size.left(), m_size.right());
  m_state(1) = random_number(m_size.top(), m_size.bottom());

  m_state(2) = random_number(m_size.left(), m_size.right());
  m_state(3) = random_number(m_size.top(), m_size.bottom());

  m_state(4) = random_number(-m_max_speed_ball, +m_max_speed_ball);
  m_state(5) = random_number(-m_max_speed_ball, +m_max_speed_ball);

  resetOthers();
}

bool HaxBall::hasOpponent() const { return m_has_opponent; }

void HaxBall::resetOthers()
{
  m_wasInLeftGoal = false;
  m_wasInRightGoal = false;

  m_num_agent_goals = 0;
  m_num_opponent_goals = 0;
}

int HaxBall::getAgentGoals() const { return m_num_agent_goals; }
int HaxBall::getOpponentGoals() const { return m_num_opponent_goals; }

void HaxBall::collision(Eigen::Ref<Eigen::Vector2d> p1, Eigen::Ref<Eigen::Vector2d> v1, double r1,
                        Eigen::Ref<Eigen::Vector2d> p2, Eigen::Ref<Eigen::Vector2d> v2, double r2) const
{
  // Work in particel 1's coordinate frame to make things easier
  Eigen::Vector2d p2_in_1 = p2 - p1;
  Eigen::Vector2d v2_in_1 = v2 - v1;

  double distance = p2_in_1.norm();
  
  // No collision -> early out such that p2 and v2 remain unchanged,
  // or if ball and player at same position (avoid zero division)
  if (distance > r1 + r2 or distance < 1e-5)
    return;
  
  // Helpers to calculate the collision
  Eigen::Vector2d normal = p2_in_1 / distance;
  Eigen::Vector2d perpen; perpen << -normal(1), normal(0);

  // Project the movement of Particle 2 on the helper
  double v_normal = normal.dot(v2_in_1);
  double v_perpen = perpen.dot(v2_in_1);

  // Reflections == Flipped normal velocity
  Eigen::Vector2d v2_new_normal = -v_normal * normal;

  // Perpendicular direction is not affected
  Eigen::Vector2d v2_new_perpen = v_perpen * perpen;

  // Superposition is new velocity after collision
  Eigen::Vector2d v2_new = v2_new_normal + v2_new_perpen;

  // Back to world frame and update the reference
  v2 = v2_new + v1;

  // Finally, fix intersection by teleporting particle p2 to the surface
  p2 = p1 + (r1 + r2) * normal;
}

double HaxBall::random_number(double low, double high)
{
  // Todo: Use the proper distribution parameter info object here
  return m_uniform_dist(m_random_engine) * (high - low) + low;
}

void HaxBall::action_map(int action, Eigen::Ref<Eigen::Vector3d> action_mapped) const
{
  switch (action)
  {
  case 0:
    action_mapped << 0.0 ,0.0, 0.0; // No op
    break;
  case 1:
    action_mapped << 0.0, 0.0, 1.0; // No move but shoot
    break;
  case 2:
    action_mapped << 0.0, 1.0, 0.0; // North
    break;
  case 3:
    action_mapped << 0.0, 1.0, 1.0; // North shoot
    break;
  case 4:
    action_mapped << -1.0, 1.0, 0.0; // North West
    break;
  case 5:
    action_mapped << -1.0, 1.0, 1.0; // North West Shoot
    break;
  case 6:
    action_mapped << -1.0, 0.0, 0.0; // West
    break;
  case 7:
    action_mapped << -1.0 , 0.0, 1.0; // West Shoot
    break;
  case 8:
    action_mapped << -1.0, -1.0, 0.0; // South West
    break;
  case 9:
    action_mapped << -1.0, -1.0, 1.0 ; // South West Shoot
    break;
  case 10:
    action_mapped << 0.0, -1.0, 0.0; // South
    break;
  case 11:
    action_mapped << 0.0, -1.0, 1.0; // South Shoot
    break;
  case 12:
    action_mapped << 1.0, -1.0, 0.0; // South East
    break;
  case 13:
    action_mapped << 1.0, -1.0, 1.0; // South East Shoot
    break;
  case 14:
    action_mapped << 1.0, 0.0, 0.0; // East
    break;
  case 15:
    action_mapped << 1.0, 0.0, 1.0; // East Shoot
    break;
  case 16:
    action_mapped << 1.0, 1.0, 0.0; // North East
    break;
  case 17:
    action_mapped << 1.0, 1.0, 1.0; // North East Shoot
    break;

  default:
    std::stringstream ss;
    ss << "Invalid action: " << action;
    throw std::runtime_error(ss.str());
  }
}
