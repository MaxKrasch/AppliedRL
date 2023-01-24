#include "Q_Agent.hpp"
#include "RewardPoint.hpp"
#include "BaseReward.hpp"
#include "greediness.hpp"

using namespace std;

Q_Agent::Q_Agent(const Eigen::Ref<const Eigen::VectorXd> &action_vector_, BaseQuantisation *quant_class, BaseReward *reward_class) : Q_table(action_vector_.rows(), quant_class->getStateSize())
{

    action_vector = action_vector_;

    int action_size = action_vector.rows();

    quant_class_ = quant_class;
    reward_class_ = reward_class;

    // initialize Q-table
    Q_table = Eigen::MatrixXd::Zero(action_size, quant_class->getStateSize());
}

int Q_Agent::policy(int state) const
{
    return 0;
}

int Q_Agent::policy(const Eigen::Ref<const Eigen::VectorXd> &obs) const
{

    int q_table_index = getQtableStateIndex(obs);

    int action_index = getBestActionIndex(q_table_index);

    Eigen::VectorXd q_slice = Q_table.col(q_table_index);

    //std::cout << "Q_Slice: \n " << q_slice.transpose() << "\n" << "Selected Action: " << action_vector(action_index) << std::endl ; // @todo find cleaner way to inspect q_slice


    return action_vector(action_index);
}

void Q_Agent::load(fs::path agent_folder)
{
    std::string filename = agent_folder.append("q_matrix.csv").c_str();
    std::ifstream file(filename);
    // This function converts input file data into the Eigen matrix format

    vector<double> matrixEntries; // for storing the matrix entries

    ifstream matrixDataFile(filename);

    string matrixRowString; // used to store a single row of the matrix
    string matrixEntry;     // used to store a single matrix entry;

    int matrixRowNumber = 0; // used to track the number of rows


    std::vector<std::vector<double>> matrix_entries;

    while (getline(matrixDataFile, matrixRowString)) // here we read a row by row of matrixDataFile and store every line into the string variable matrixRowString
    {
        stringstream matrixRowStringStream(matrixRowString); // convert matrixRowString that is a string to a stream variable.
        std::vector<double> matrix_row_entries;

        while (getline(matrixRowStringStream, matrixEntry, ',')) // here we read pieces of the stream matrixRowStringStream until every comma, and store the resulting character into the matrixEntry
        {
            matrix_row_entries.push_back(stod(matrixEntry)); // here we convert the string to double and fill in the row vector storing all the matrix entries
        }
        matrix_entries.push_back(matrix_row_entries);
        matrixRowNumber++; // update the column numbers
    }

    int matrixColNumber = matrix_entries[0].size();

    Q_table.resize(matrixRowNumber, matrixColNumber);

    for (int i = 0; i < matrixRowNumber; i++)
    {
        Eigen::VectorXd row = Eigen::Map<Eigen::VectorXd, Eigen::Unaligned>(matrix_entries[i].data(), matrix_entries[i].size());
        Q_table.row(i) = row;
    }

    file.close();
}

void Q_Agent::save(fs::path agent_folder) const
{
    std::string filename = agent_folder.append("q_matrix.csv").c_str();
    std::remove(filename.c_str()); // remove file if it already exists
    const static Eigen::IOFormat CSVFormat(Eigen::FullPrecision, Eigen::DontAlignCols, ", ", "\n");
    std::ofstream file(filename);
    if (file.is_open())
    {
        file << Q_table.format(CSVFormat);
    }
    else
    {
        std::cout << "error with saving q_matrix\n";
    }
    file.close();
}

std::vector<double> Q_Agent::train(int epochs, double alpha, double gamma, HaxBall &sim, int trajectory_length, Greediness* greediness)
{
    // In this function the agent is trained for number=epochs trajectories in the environment

    int action_cur = 0;         // action in current time step
    int action_next = 0;        // action in next time step
    int q_table_index_next = 0; // index that describes the row index in the Q-table depending on the current state (rows=states and cols=actions in Q-table)

    double accumulated_reward = 0.0; // cumulative reward that is calculated over all trajectories
    double trajectory_reward = 0.0;  // cumulative reward over all time steps that is calculated for each trajectory

    Eigen::Vector<double, 6> state;      // current time steps state vector
    Eigen::Vector<double, 6> state_next; // next time steps state vector

    Eigen::VectorXd q_slice; // slice of the q_table with the Q-values for every action of a single state.

    // Greediness
    int stored_action =0;
    double prob = 0;
    double epsilon = 0;
    int timestep=0;

    std::vector<double> reward_vector;

    // multi stepping
    int step_number = 100;
    double rew_multi = 0;
    double gamma_multi = 0.95;

    // omp_set_num_threads(8); //todo: set number of threads

    //#pragma omp parallel for
    for (int i = 0; i < epochs; ++i) // for loop over all epochs/trajectories
    {
        sim.getState(state);                            // store current state of environment in state variable
        int q_table_index = getQtableStateIndex(state); // determine index of row in Q-table depending on current state

        accumulated_reward = 0;

        for (int j = 0; j < trajectory_length; ++j) // for loop over whole trajectory
        {
            trajectory_reward = 0;


            // Determine action
            epsilon = greediness->getProbabilityForRandomAction(i, timestep);

            prob = rand() / (RAND_MAX + 1.0);   // variable that is assigned a random value in [0, 1] each time step and is compared to the threshold epsilon to determine if a random action is chosen or not

            if(greediness->useSameAction()){
                action_cur = stored_action;
            }else if(prob < epsilon){ // random action
                action_cur = getRandomActionIndex();
            }else{
                action_cur = getBestActionIndex(q_table_index);
            }

            stored_action = action_cur;


            // take action in the environment
            sim.step(action_vector(action_cur));
            timestep++;

            // quantize new position
            sim.getState(state_next);
            q_table_index_next = getQtableStateIndex(state_next); // get row index for Q-table of next state

            // receive reward
            accumulated_reward += reward(state, action_cur, state_next);
            trajectory_reward += reward(state, action_cur, state_next);

            // find best next action in next state
            action_next = getBestActionIndex(q_table_index);

            // storing for multi stepping


            // update value in Q-table

            Q_table(action_cur, q_table_index) = Q_table(action_cur, q_table_index) + alpha * (reward(state, action_cur, state_next) + gamma * Q_table(action_next, q_table_index_next) - Q_table(action_cur, q_table_index));


            // reset variables
            q_table_index = q_table_index_next;
            state = state_next;
        }
        sim.reset();
        if (i % 1000 == 0)
        {
            std::cout << "epoch: " << i << " accumulated reward: " << accumulated_reward << " Probability of random action: " << greediness->getProbabilityForRandomAction(i, timestep) << "\n";
            reward_vector.push_back(accumulated_reward);
        }

        //#pragma omp critical
    }
    return reward_vector;
}

int Q_Agent::getQtableStateIndex(const Eigen::Ref<const Eigen::VectorXd> &state) const
{
    return quant_class_->getQtableStateIndex(state);
}

double Q_Agent::reward(const Eigen::Ref<const Eigen::VectorXd> &o, int a, const Eigen::Ref<const Eigen::VectorXd> &o_prime) const
{
    return reward_class_->get_reward(o, a, o_prime);
}

std::string Q_Agent::getType() const
{
    std::string type = "Q_Agent\n";
    type += quant_class_->getType() + "\n";
    type += reward_class_->getType();
    return type;
}

Eigen::VectorXd Q_Agent::getActionVector() const
{
    return action_vector;
}

void Q_Agent::printInfo() const
{
    std::cout << "TODO \n"; // @todo print info
}

template<typename Iter, typename RandomGenerator>
Iter select_randomly(Iter start, Iter end, RandomGenerator& g) {
    std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
    std::advance(start, dis(g));
    return start;
}

template<typename Iter>
Iter select_randomly(Iter start, Iter end) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return select_randomly(start, end, gen);
}

int Q_Agent::getBestActionIndex(int stateIndex) const
{
    Eigen::VectorXd q_slice = Q_table.col(stateIndex);

    std::vector<int> action_indices;
    double max_value = q_slice.maxCoeff();

    for(int i=0; i<q_slice.size(); i++){
        if(q_slice[i]==max_value){
            action_indices.push_back(i);
        }
    }

    int action_index = *select_randomly(action_indices.begin(), action_indices.end());

    return action_index;
}

int Q_Agent::getRandomActionIndex() const
{
    int index = rand() % action_vector.size();

    return index;
}

double Q_Agent::get_multi_step_reward(std::vector<double> rewards, int step_number, double gamma_multi){
    double rew_multi = 0;
    double reward_tmp = 0;

    for(int i=0; i<rewards.size(); i++){
        reward_tmp = pow(gamma_multi,i)*rewards[i];
        rew_multi += reward_tmp;
    }


    rew_multi = rew_multi/step_number;
    return rew_multi;
}
