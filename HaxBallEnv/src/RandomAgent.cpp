#include "RandomAgent.h"

#include <cmath>
#include <iostream>

#include <QDebug>

#include <omp.h>

RandomAgent::RandomAgent()
{

}
RandomAgent::~RandomAgent()
{

}

int RandomAgent::policy(int state) const
{
  // Shoot + Move North West
  return 4;
}

int RandomAgent::policy(const Eigen::Ref<const Eigen::VectorXd>& obs) const
{
  return policy(0);
}

void RandomAgent::training(int length, int trajectories, int threads)
{
  omp_set_num_threads(threads);

#pragma omp parallel for
  for (int i = 0; i < trajectories; ++i)
  {
    training_worker(length);
  }
}
void RandomAgent::training_worker(int length)
{
  // Training code goes here, be aware that multiple threads are active in here
}

double RandomAgent::reward(const Eigen::Ref<const Eigen::VectorXd>& o, int a, const Eigen::Ref<const Eigen::VectorXd>& o_prime) const
{
  // Put your reward here

  return 0.0;
}

