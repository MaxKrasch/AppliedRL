#include "trainer.hpp"
#include "Agents/include/Q_Agent.hpp"
#include <vector>

Trainer::Trainer()
{
    agent_is_loaded_ = false;

    project_path_ = fs::current_path().parent_path();
    saved_agents_path_ = project_path_.append("Storage");
}


void Trainer::loadAgent(BaseRLAgent *agent){
    agent_ = agent;
    agent_is_loaded_ = true;
    std::cout << "Loading Agent. ";
    if(document_agent_){
        std::cout << "Agent will be documented in " << agent_path_.c_str() << ".\n";
    }else{
        std::cout << "Agent will NOT be documented.\n";
    }
}


bool Trainer::loadAgentFromFile(std::string folder_name){
    agent_path_ = saved_agents_path_.append(folder_name);
    document_agent_ = true;

    std::cout << "Loading agent from " << agent_path_.c_str() << ".\n";

    agent_info_file_ = combine_paths(agent_path_, "agent.info");
    
    std::ifstream info_file_stream(agent_info_file_);

    std::string agent_type;
    std::getline(info_file_stream, agent_type);

    std::cout << "Creating agent of type " << agent_type << "...\n";

    if(agent_type == "Q_Agent"){
        // Load action vector
        std::cout << "Loading action_vector...";
        fs::path action_vector_file = combine_paths(agent_path_, "action_vector.info");
        Eigen::VectorXd action_vector = load_vectorxd_from_file(action_vector_file);

        std::cout << "   Successful!" << std::endl;

        
        
        std::string quantisation_type;
        std::getline(info_file_stream, quantisation_type);

        BaseQuantisation* quantisation;

        std::cout << "Trying to create object..."<< std::endl;

        if(quantisation_type == "Quantisation"){
            quantisation = new Quantisation(agent_path_);
        }else{
            // Load Quantisation
            std::cout << "Loading quantisation..."<< std::endl;
            fs::path quantisation_config_file = combine_paths(agent_path_, "quantisation.info");
            std::cout << "Loading vector from file..."<< std::endl;
            std::vector<double> quant_config = load_std_vector_from_file(quantisation_config_file);

            if(quantisation_type == "QuantGridPlayer"){
                quantisation = new QuantGridPlayer(quant_config[0], quant_config[1]);
            }else if(quantisation_type == "QuantGridPlayerBall"){
                quantisation = new QuantGridPlayerBall(quant_config[0], quant_config[1], quant_config[2], quant_config[3]);
            }else if(quantisation_type == "QuantGridPlayerAngleBall"){
                quantisation = new QuantGridPlayerAngleBall(quant_config[0], quant_config[1]);
            }else{
                std::cout << "Unknown Quantisation";
                return false;
            }
        }
        std::cout << "   Successful!" << std::endl;

        // Load Reward
        std::cout << "Loading reward...";
        fs::path reward_config_file = combine_paths(agent_path_, "reward.info");
        std::vector<double> reward_config = load_std_vector_from_file(reward_config_file);
        
        std::string reward_type;
        std::getline(info_file_stream, reward_type);

        BaseReward* reward;

        if(reward_type == "RewardPoint"){
            Eigen::Vector2d vecP;
            vecP << reward_config[1], reward_config[2];
            reward = new RewardPoint(reward_config[0], vecP);
        }else if(reward_type == "RewardBall"){
            reward = new RewardBall(reward_config[0]);
        }else if(reward_type == "RewardGoal"){
            reward = new RewardGoal(reward_config[0], reward_config[1], reward_config[2]);
        }else{
            std::cout << "Unknown Quantisation";
            return false;
        }
        std::cout << "   Successful!" << std::endl;

        agent_ = new Q_Agent(action_vector, quantisation, reward);
    }else{
        std::cout << "Agent type " << agent_type << " is unknown. Abort.";
        return false;
    }



    std::cout << "Loading saved parameters...\n";
    agent_->load(agent_path_);
    document_agent_ = true;
    agent_is_loaded_ = true;
    
    std::cout << "Successfully loaded Agent:\n";

    agent_->printInfo();

    return true;
}

bool Trainer::startDocumentation(std::string folder_name){
    // Create Folder for saving agent
    agent_path_ = saved_agents_path_.append(folder_name);
    bool success = fs::create_directory(agent_path_);
    if (not success)
    {
        throw std::invalid_argument("Folder cannot be created. Does it already exist?");
    }


    // Write Agent Info File with info about the components

    agent_info_file_ = combine_paths(agent_path_, "agent.info");
    std::ofstream info_file_stream(agent_info_file_);
    // Write agent, quantisation and reward type
    info_file_stream << agent_->getType();



    // Write action vector
    fs::path action_vector_file = combine_paths(agent_path_, "action_vector.info");
    save_vectorxd_to_file(action_vector_file, agent_->action_vector);


    // Write Quantisation Configuration
    if(agent_->quant_class_->getType() == "Quantisation"){
        Quantisation* quantisation = static_cast<Quantisation*>(agent_->quant_class_);
        quantisation->saveConfiguration(agent_path_);
    }else{
        fs::path quantisation_config_file = combine_paths(agent_path_, "quantisation.info");
        save_std_vector_to_file(quantisation_config_file, agent_->quant_class_->getConfiguration());
    }


    // Write Reward config
    fs::path reward_config_file = combine_paths(agent_path_, "reward.info");
    save_std_vector_to_file(reward_config_file, agent_->reward_class_->getConfiguration());


    // TODO: Save training info

    agent_->save(agent_path_);
    document_agent_ = true;
    std::cout << "Started Documentation. \n";
    return true;
}

bool Trainer::isDocumenting() const{
    return document_agent_;
}

void Trainer::stopDocumentation(){
    document_agent_ = false;
}

double Trainer::train(int epochs, double alpha, double gamma, double epsilon, int trajectory_length, Greediness* greediness, bool opponent){
    HaxBall haxball(opponent);
    agent_->reward_class_->setSim(&haxball);

    auto training_info_file = combine_paths(agent_path_, "training.info");
    std::ofstream training_file_stream(training_info_file);
    // Write agent, quantisation and reward type
    training_file_stream << "Epochs: " << epochs;
    training_file_stream << "Alpha: " << alpha;
    training_file_stream << "Gamma: " << gamma;
    training_file_stream << "Epsilon: " << epsilon;
    training_file_stream << "Trajectory Length: " << trajectory_length;
    training_file_stream << greediness->getInfo();

    std::vector<double> reward = agent_->train(epochs, alpha, gamma, haxball, trajectory_length, greediness);


    auto training_reward_file = combine_paths(agent_path_, "training_reward.info");
    save_std_vector_to_file(training_reward_file, reward);

    if(document_agent_){
        agent_->save(agent_path_);
        std::cout << "Saving trained agent...";
    }
    return accumulate(reward.begin(),reward.end(),0);
}

void Trainer::render(int argc, char** argv, bool opponent)
{
    // Create a shared instance of haxball. Used by the gui AND the trainer simultaneously
    std::shared_ptr<HaxBall> haxball = std::make_shared<HaxBall>(opponent); 

    // Create Gui
    QApplication app(argc, argv);
    HaxBallGui gui(*agent_, haxball);
    gui.show();

    gui.playGame(10.0, 50000);

    app.exec();
}

void Trainer::render(int argc, char** argv, bool opponent, print_info_function func)
{
    // Create a shared instance of haxball. Used by the gui AND the trainer simultaneously
    std::shared_ptr<HaxBall> haxball = std::make_shared<HaxBall>(opponent); 

    HaxBall* haxball_pointer = haxball.get();
    agent_->reward_class_->setSim(haxball_pointer);

    // Create Gui
    QApplication app(argc, argv);
    HaxBallGui gui(*agent_, haxball);
    gui.show();

    // Start thread that prints the reward in the background
    std::thread t1(func, std::ref(haxball), std::ref(agent_));

    gui.playGame(10.0, 50000);

    app.exec();

    t1.join();  // Wait for thread to finish
}

void Trainer::benchmarkAgent(bool opponent){
    
    int num_iterations = 2500;
    int trajectory_length=0;


    std::cout << "Starting benchmark ";

    if(opponent==true){
        std::cout << "with opponent  " << std::endl;
        trajectory_length = 20e3;
    }else{
        std::cout << "without opponent " << std::endl;
        trajectory_length = 50e3;
    }

    std::vector<int> agent_goals;
    std::vector<int> opponent_goals;
    std::vector<int> goal_difference;

    
    Eigen::Vector<double, 6> state;     
    int action=0; 

    for(int i=0; i<num_iterations; i++){
        HaxBall haxball(opponent);

        for(int j=0; j<trajectory_length; j++){
            haxball.getState(state);
            vector<int> v{10,21,24,13};

            int random = rand() % v.size();
            int sel_elem = v[random];
            action = 0; //agent_->policy(state);
            haxball.step(action);
        }

        agent_goals.push_back(haxball.getAgentGoals());
        opponent_goals.push_back(haxball.getOpponentGoals());
        goal_difference.push_back(haxball.getAgentGoals() - haxball.getOpponentGoals());

        haxball.reset();
    }

    int max_goals_shot = *std::max_element(agent_goals.begin(), agent_goals.end());
    int max_goal_difference = *std::max_element(goal_difference.begin(), goal_difference.end());
    int min_goals_shot = *std::min_element(agent_goals.begin(), agent_goals.end());

    double agent_goals_per_iteration = std::accumulate(agent_goals.begin(), agent_goals.end(), 0) / (double)num_iterations;
    double opponent_goals_per_iteration = std::accumulate(opponent_goals.begin(), opponent_goals.end(), 0) / (double)num_iterations;
    double goal_difference_per_iteration = std::accumulate(goal_difference.begin(), goal_difference.end(), 0) / (double)num_iterations;

    std::cout << "*******************************************************************************" << std::endl;
    std::cout << "*************************** Benchmark Results**********************************" << std::endl;
    std::cout << "*******************************************************************************" << std::endl;
    std::cout << "Max Agent Goals in one Iteration: " << max_goals_shot << std::endl;
    std::cout << "Max Goal Difference in one Iteration: " << max_goal_difference << std::endl;
    std::cout << "Min Agent Goals in one Iteration:  " << min_goals_shot << std::endl;
    std::cout << "Agent Goals per Iteration: " << agent_goals_per_iteration << std::endl;
    std::cout << "Opponent Goals per Iteration: " << opponent_goals_per_iteration << std::endl;
    std::cout << "Goal Difference per Iteration: " << goal_difference_per_iteration << std::endl;

    if(document_agent_){
        auto benchmark_info_file = combine_paths(agent_path_, "benchmark.info");
        std::ofstream benchmark_file_stream(benchmark_info_file);

        benchmark_file_stream << "Opponent: " << opponent << std::endl;
        benchmark_file_stream << "Max Agent Goals in one Iteration: " << max_goals_shot << std::endl;
        benchmark_file_stream << "Max Goal Difference in one Iteration: " << max_goal_difference << std::endl;
        benchmark_file_stream << "Min Agent Goals in one Iteration: " << min_goals_shot << std::endl;
        benchmark_file_stream << "Agent Goals per Iteration: " << agent_goals_per_iteration << std::endl;
        benchmark_file_stream << "Opponent Goals per Iteration: " << opponent_goals_per_iteration << std::endl;
        benchmark_file_stream << "Goal Difference per Iteration: " << goal_difference_per_iteration << std::endl;
    }
}
