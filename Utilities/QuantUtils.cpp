#include "QuantUtils.hpp"


/**
 * @brief Returns the angle between the imaginary line that goes from goal to (x,y) and th eline that goes to teh top. Angle is in the range [0, pi]
 * 
 * @param x 
 * @param y 
 * @return double 
 */
double getAngleFromGoal_rad(double x, double y){
    double relative_x = x - 4.0;
    double relative_y = y;

    double angle = -atan2(relative_x, relative_y);
    // angle is between 0 and pi. 0 is top. 

    if(angle <0){
        if(angle>-0.2){
            angle=0;
        }else{
            angle=M_PI;
        }
    }

    return angle;
}

double getAngleFromGoal_deg(double x, double y){
    double angle_rad = getAngleFromGoal_rad(x,y);
    double angle_degree = (angle_rad * 180) / M_PI;

    return angle_degree;
}

double getAngle_rad(double x, double y){

    double angle = atan2(x, y);
    // angle is between -pi and pi. 0 is top. negative is left side, positive is right side.

    return angle;
}

int getAngleFromGoalQuantisation(double x, double y, int quant_size){
    double angle = getAngleFromGoal_rad(x,y);

    double step = M_PI / quant_size;

    int quant = (int)(angle/step);
    return quant;
}

int getBallSpeedQuantisation(double speed, int num_bins){
    double max_speed_ball = 3.0*0.075;

    double step = max_speed_ball / num_bins;

    int quant = (int)(speed / step);

    return quant;
}

int getScalarQuantisation(double x, double min, double max, int num_bins){
    double step = (max-min)/(double)num_bins;
    x = x-min;
    int quant = x / step;
    if(quant >= num_bins){
        quant = num_bins-1;
    }
    if(quant < 0){
        std::cout << "Warning: An Index was below 0. Index was: " << quant << std::endl;
        quant=0;
    }
    return quant;
}

