#ifndef _BASE_QUANT_H_
#define _BASE_QUANT_H_


#include "BaseAgent.h"
#include "HaxBall.h"

#include "Eigen/Dense"
#include <vector>
#include <string>

class BaseQuantisation{
    public:

        virtual int getQtableStateIndex(const Eigen::Ref<const Eigen::VectorXd>& state) const=0;

        virtual int getStateSize() const=0;

        virtual std::string getType() const =0;

        virtual std::vector<double> getConfiguration() const=0;
};


#endif // _BASE_QUANT_H_