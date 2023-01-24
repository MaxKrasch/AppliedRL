#include "HaxBallGui.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QApplication>
#include <QDesktopWidget>

#include <iostream>
#include <QDebug>

HaxBallGui::HaxBallGui(const BaseAgent& agent, std::shared_ptr<HaxBall> world, QWidget* parent, Qt::WindowFlags flags): QMainWindow(parent, flags),
  m_world(world), m_agent(agent),
  m_timer(0), m_render_step_counter(0),
  m_view(0), m_scene(0),
  m_player(0), m_ball(0), m_opponent(0),
  m_keyboard_policy_active(false),
  m_move_up(false), m_move_down(false), m_move_left(false), m_move_right(false), m_shoot(false)
{
  QWidget* centeral_widget = new QWidget(this);
  this->setCentralWidget(centeral_widget);

  QVBoxLayout* layout_master = new QVBoxLayout(this);
  centeral_widget->setLayout(layout_master);

  m_view = new QGraphicsView(this);
  layout_master->addWidget(m_view);

  m_scene = new QGraphicsScene(this);
  m_view->setScene(m_scene);

  QHBoxLayout* layout_counter = new QHBoxLayout(this);
  layout_master->addLayout(layout_counter);

  m_goalcounter_agent = new QLCDNumber(this);
  m_goalcounter_opponent = new QLCDNumber(this);

  m_goalcounter_agent->setSegmentStyle(QLCDNumber::SegmentStyle::Flat);
  m_goalcounter_agent->setFrameStyle(QFrame::Shape::NoFrame);

  m_goalcounter_opponent->setSegmentStyle(QLCDNumber::SegmentStyle::Flat);
  m_goalcounter_opponent->setFrameStyle(QFrame::Shape::NoFrame);

  layout_counter->addWidget(m_goalcounter_agent);
  layout_counter->addWidget(m_goalcounter_opponent);

  QCheckBox* box = new QCheckBox("Activate keyboard input", this);

  layout_master->addWidget(box);
  box->setChecked(false);
  box->setFocusPolicy(Qt::FocusPolicy::NoFocus);

  createSceneContent();

  m_timer = new QTimer(this);
  connect(m_timer, &QTimer::timeout, this, &HaxBallGui::playGameStep);
  connect(box, &QCheckBox::clicked, this, &HaxBallGui::setKeyBoardPolicyActive);

  if (QApplication::desktop()->screenGeometry().width() > 2000)
  {
    this->resize(800, 500);
  } else
  {
    this->resize(400, 300);
  }

  this->setWindowTitle("Hax Ball");
}

HaxBallGui::HaxBallGui(const BaseAgent& agent, QWidget* parent, Qt::WindowFlags flags):
  HaxBallGui(agent, std::make_shared<HaxBall>(), parent, flags) {}

HaxBallGui::~HaxBallGui()
{

}

void HaxBallGui::playGame(double speedup, int steps)
{
  m_render_step_counter = steps;
  m_timer->start(1000.0 * m_world->getTimeDelta() / speedup );
}

void HaxBallGui::keyPressEvent(QKeyEvent* event) { handleKeyEvent(event, true); }
void HaxBallGui::keyReleaseEvent(QKeyEvent* event){ handleKeyEvent(event, false); }

void HaxBallGui::createSceneContent()
{
  m_scene->clear();

  // Pens and brushes as needed below
  QBrush b_blue(Qt::blue);
  QBrush b_red(Qt::red);
  QBrush b_green(Qt::green);
  QBrush b_black(Qt::black);
  QBrush b_white(Qt::white);
  QBrush b_trans(Qt::transparent);
  QBrush b_grey(Qt::gray);

  QPen p_blue(b_blue, 0.05);
  QPen p_red(b_red, 0.05);
  QPen p_grey(b_grey, 0.05);

  QPen p_black(b_black, 0.05);
  QPen p_white(b_white, 0.05);

  QRectF s = m_world->getSize();
  qreal r;

  // Green background
  m_scene->addRect(s, p_black, b_green);

  // Goal area
  m_scene->addRect(m_world->getGoalLeft(), p_white, b_white );
  m_scene->addRect(m_world->getGoalRight(), p_white, b_white );

  // The two white rectangles (appears as one big rectangle with vertical line in the middle)
  m_scene->addRect(QRectF(QPointF(0.9 * s.left(), 0.9 * s.top()),
                          QPointF(0.0, 0.9 * s.bottom())),
                   p_white, b_trans);
  m_scene->addRect(QRectF(QPointF(0.0, 0.9 * s.top()),
                          QPointF(0.9 * s.right(), 0.9 * s.bottom())),
                   p_white, b_trans);

  // The circle in the middle
  m_scene->addEllipse(-1.0, -1.0, 2.0, 2.0, p_white, b_trans);

  // Player and ball, result is stored in class to be able to update the position
  // Spawn centered at zero position, otherwise you have to compensate the offset when setting the position later on
  r = m_world->getRadiusPlayer();
  m_player = m_scene->addEllipse(-r, -r, 2.0 * r, 2.0 * r, p_blue, b_blue);

  r = m_world->getRadiusBall();
  m_ball = m_scene->addEllipse(-r, -r, 2.0 * r, 2.0 * r, p_grey, b_grey);

  if( m_world->hasOpponent())
  {
    r = m_world->getRadiusPlayer();
    m_opponent = m_scene->addEllipse(-r, -r, 2.0 * r, 2.0 * r, p_red, b_red);
  }
}

void HaxBallGui::showEvent(QShowEvent*){fitInView();}
void HaxBallGui::resizeEvent(QResizeEvent* event){fitInView();}

void HaxBallGui::fitInView()
{
  m_scene->setSceneRect(m_world->getSize());
  m_view->fitInView(m_world->getSize());
}

void HaxBallGui::updateViewFromWorld()
{
  if(m_player) m_player->setPos(m_world->getPlayerPos());
  if(m_ball) m_ball->setPos(m_world->getBallPos());
  if(m_opponent) m_opponent->setPos(m_world->getOpponentPos());
}

int HaxBallGui::keyBoardPolicy() const
{
  // Pressing up
  if(m_move_up and !m_move_left and !m_move_down and !m_move_right)
  {
    if(m_shoot)
      return 3;
    else
      return 2;
  }

  // Pressing left up
  if(m_move_up and m_move_left and !m_move_down and !m_move_right)
  {
    if(m_shoot)
      return 5;
    else
      return 4;
  }

  // Pressing left
  if(!m_move_up and m_move_left and !m_move_down and !m_move_right)
  {
    if(m_shoot)
      return 7;
    else
      return 6;
  }

  // Pressing down left
  if(!m_move_up and m_move_left and m_move_down and !m_move_right)
  {
    if(m_shoot)
      return 9;
    else
      return 8;
  }

  // Pressing down
  if(!m_move_up and !m_move_left and m_move_down and !m_move_right)
  {
    if(m_shoot)
      return 11;
    else
      return 10;
  }

  // Pressing down right
  if(!m_move_up and !m_move_left and m_move_down and m_move_right)
  {
    if(m_shoot)
      return 13;
    else
      return 12;
  }

  // Pressing right
  if(!m_move_up and !m_move_left and !m_move_down and m_move_right)
  {
    if(m_shoot)
      return 15;
    else
      return 14;
  }

  // Pressing right up
  if(m_move_up and !m_move_left and !m_move_down and m_move_right)
  {
    if(m_shoot)
      return 17;
    else
      return 16;
  }

  if(m_shoot)
    return 1;
  else
    return 0;
}

void HaxBallGui::handleKeyEvent(QKeyEvent* event, bool pressed)
{
  if (event->key() == Qt::Key::Key_W)
     m_move_down = pressed;

  if (event->key() == Qt::Key::Key_S)
    m_move_up = pressed;

  if (event->key() == Qt::Key::Key_A)
    m_move_left = pressed;

  if (event->key() == Qt::Key::Key_D)
    m_move_right = pressed;

  if (event->key() == Qt::Key::Key_Space || event->key() == Qt::Key::Key_Q || event->key() == Qt::Key::Key_E)
    m_shoot = pressed;
}

void HaxBallGui::playGameStep()
{ 
  // Render the current state in which the action is executed, not the state after the execution
  updateViewFromWorld();

  Eigen::VectorXd o(m_world->getNO());
  m_world->getState(o);
  // std::cout << o.transpose() << std::endl;  // If you want to see the observations

  if(m_keyboard_policy_active)
    m_world->step(keyBoardPolicy());
   else
    m_world->step(m_agent.policy(o));

  m_goalcounter_agent->display(m_world->getAgentGoals());
  m_goalcounter_opponent->display(m_world->getOpponentGoals());

  m_render_step_counter--;

  // Don't try to use an unsigned int here ...
  if(m_render_step_counter <= 0)
  {
    m_timer->stop();
    this->close();
  }
}

void HaxBallGui::setKeyBoardPolicyActive(bool is_active) { m_keyboard_policy_active = is_active; }
