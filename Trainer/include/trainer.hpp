#ifndef TRAINER_H
#define TRAINER_H

#include <iostream>
#include <fstream>

#include <QApplication>


#include <filesystem>

#include <string>

#include "HaxBall.h"
#include "HaxBallGui.h"

#include <stdlib.h>     /* srand, rand */


#include "BaseRLAgent.hpp"
#include "Q_Agent.hpp"

#include "BaseQuantisation.hpp"
#include "QuantGridPlayer.hpp"
#include "QuantGridPlayerBall.hpp"
#include "QuantGridPlayerAngleBall.hpp"
#include "Quantisation.hpp"

#include "BaseReward.hpp"
#include "RewardPoint.hpp"
#include "RewardBall.hpp"
#include "RewardGoal.hpp"

#include "greediness.hpp"

#include <thread>

#include "file_handling.hpp"





class Trainer{


    public:
        Trainer();

        /**
         * @brief Load a previously created agent into the trainer. Replaces any old agents that might have been loaded into the trainer.
         * 
         * @param agent 
         */
        void loadAgent(BaseRLAgent *agent);

        
        /**
         * @brief Loads a saved agent from the folder folder_name in the Storage folder.
         * 
         * @param folder_name 
         * @return true When successfull
         * @return false when loading failed
         */
        bool loadAgentFromFile(std::string folder_name);


        /**
         * @brief Starts the documentation of the loaded agent. Meaning it will create a new folder to save the agent into. Throws an error if that folder already exists.
         * 
         * @param folder_name 
         * @return true 
         * @return false 
         */
        bool startDocumentation(std::string folder_name);


        /**
         * @brief 
         * 
         * @return true when the loaded agent is being documented.
         * @return false when the loaded agent is NOT being documented.
         */
        bool isDocumenting() const;


        /**
         * @brief After calling the loaded agent will not save any changes or trainings to file.
         * 
         */
        void stopDocumentation();

        /**
         * @brief 
         * 
         * @param epochs 
         * @param alpha Learning Rate
         * @param gamma Discount Factor
         * @param epsilon probability of choosing a random action
         * @return double 
         */
        double train(int epochs, double alpha, double gamma, double epsilon, int trajectory_length, Greediness* greediness, bool opponent);

        typedef void (*print_info_function)(std::shared_ptr<HaxBall> &haxball, BaseRLAgent *agent);


        /**
         * @brief Renders the HaxBall Game and controls the player with the policy of the loaded agent.
         * 
         * @param argc 
         * @param argv 
         */
        void render(int argc, char** argv, bool opponent);        


        /**
         * @brief Renders the HaxBall Game and controls the player with the policy of the loaded agent. Will also regularly call a function to print additional details.
         * 
         * @param argc 
         * @param argv 
         * @param func Function for printing additional details.
         */
        void render(int argc, char** argv, bool opponent, print_info_function func);

        void benchmarkAgent(bool opponent);

        BaseQuantisation * loadQuantisation();


    private:

        

        BaseRLAgent* agent_;

        // Settings
        bool document_agent_;    /** when false, no documentation is generated */
        bool agent_is_loaded_;

        // Filepaths
        fs::path project_path_; /** The path to the project folder.  */
        fs::path saved_agents_path_;    /** Path to the folder where all agents are saved */
        fs::path agent_path_;
        fs::path agent_info_file_;
        fs::path training_info_file_;
        fs::path quantisation_info_file_;

        
        


};




#endif /* TRAINER_H */