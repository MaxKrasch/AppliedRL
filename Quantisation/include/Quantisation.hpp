#ifndef _QUANTISATION_H_
#define _QUANTISATION_H_


#include "BaseAgent.h"
#include "HaxBall.h"

#include "Eigen/Dense"

#include "BaseQuantisation.hpp"
#include "Quant.hpp"
#include <iostream>

class Quantisation : public BaseQuantisation{

    public:

        Quantisation(std::vector<Quant*> quants_);
        Quantisation(fs::path path_to_agent_folder);

        int getQtableStateIndex(const Eigen::Ref<const Eigen::VectorXd>& state) const;

        int getStateSize() const;

        std::string getType() const;

        std::vector<double> getConfiguration() const;

        void saveConfiguration(fs::path path_to_agent_folder);


    private:
        std::vector<Quant*> quants;

        
};


#endif // _QUANTISATION_H_