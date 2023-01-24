#ifndef _QUANT_UTILS_H
#define _QUANT_UTILS_H

#include <vector>
#include <math.h>
#include "HaxBall.h"
#include <iostream>

//int getAngleQuant(double rel_x, double rel_y, std::vector<double> angle_limits);

double getAngleFromGoal_rad(double x, double y);

double getAngleFromGoal_deg(double x, double y);

/**
 * @brief Get the Angle of a coordinate relative to the zero point. Returned angle is between -pi and pi. 0 is top. negative is left side, positive is right side.
 * 
 * @param x 
 * @param y 
 * @return double 
 */
double getAngle_rad(double x, double y);

int getBallSpeedQuantisation(const Eigen::Ref<const Eigen::VectorXd> &state, int num_bins);

int getScalarQuantisation(double x, double min, double max, int num_bins);

#endif // _QUANT_UTILS_H