#ifndef _QUANT_H_
#define _QUANT_H_


#include "BaseAgent.h"
#include "HaxBall.h"

#include "Eigen/Dense"
#include <vector>
#include <string>

#include "file_handling.hpp"
#include "QuantUtils.hpp"

class Quant{
    public:

        virtual int getQuantisation(const Eigen::Ref<const Eigen::VectorXd>& state) const=0;

        virtual int getStateSize() const=0;

        virtual std::string getType() const =0;

        virtual void saveConfiguration(fs::path path_to_agent_folder) const=0;
};

/**
 * @brief Quantizes the absolute ball velocity into same size bins between 0 and the maximmum ball speed.
 * 
 */
class QuantVelocity : public Quant{
    public:
        QuantVelocity(int num_bins);
        QuantVelocity(fs::path path_to_agent_folder);
        int getQuantisation(const Eigen::Ref<const Eigen::VectorXd>& state) const;
        int getStateSize() const;
        std::string getType() const;
        void saveConfiguration(fs::path path_to_agent_folder) const;

    private:
        int num_bins;
        const HaxBall haxball;
};


/**
 * @brief Quantizes the x ball velocity into same size bins between 0 and the maximmum ball speed.
 * 
 */
class QuantVelocityX : public Quant{
    public:
        QuantVelocityX(int num_bins);
        QuantVelocityX(fs::path path_to_agent_folder);
        int getQuantisation(const Eigen::Ref<const Eigen::VectorXd>& state) const;
        int getStateSize() const;
        std::string getType() const;
        void saveConfiguration(fs::path path_to_agent_folder) const;

    private:
        int num_bins;
        const HaxBall haxball;
};

/**
 * @brief Quantizes the y ball velocity into same size bins between 0 and the maximmum ball speed.
 * 
 */
class QuantVelocityY : public Quant{
    public:
        QuantVelocityY(int num_bins);
        QuantVelocityY(fs::path path_to_agent_folder);
        int getQuantisation(const Eigen::Ref<const Eigen::VectorXd>& state) const;
        int getStateSize() const;
        std::string getType() const;
        void saveConfiguration(fs::path path_to_agent_folder) const;

    private:
        int num_bins;
        const HaxBall haxball;
};

class QuantPlayerPosX : public Quant{
    public:
        QuantPlayerPosX(int num_bins);
        QuantPlayerPosX(fs::path path_to_agent_folder);
        int getQuantisation(const Eigen::Ref<const Eigen::VectorXd>& state) const;
        int getStateSize() const;
        std::string getType() const;
        void saveConfiguration(fs::path path_to_agent_folder) const;

    private:
        int num_bins;
        const HaxBall haxball;
};

class QuantPlayerPosY : public Quant{
    public:
        QuantPlayerPosY(int num_bins);
        QuantPlayerPosY(fs::path path_to_agent_folder);
        int getQuantisation(const Eigen::Ref<const Eigen::VectorXd>& state) const;
        int getStateSize() const;
        std::string getType() const;
        void saveConfiguration(fs::path path_to_agent_folder) const;

    private:
        int num_bins;
        const HaxBall haxball;
};

class QuantBallPosX : public Quant{
    public:
        QuantBallPosX(int num_bins);
        QuantBallPosX(fs::path path_to_agent_folder);
        int getQuantisation(const Eigen::Ref<const Eigen::VectorXd>& state) const;
        int getStateSize() const;
        std::string getType() const;
        void saveConfiguration(fs::path path_to_agent_folder) const;

    private:
        int num_bins;
        const HaxBall haxball;
};

class QuantBallPosY : public Quant{
    public:
        QuantBallPosY(int num_bins);
        QuantBallPosY(fs::path path_to_agent_folder);
        int getQuantisation(const Eigen::Ref<const Eigen::VectorXd>& state) const;
        int getStateSize() const;
        std::string getType() const;
        void saveConfiguration(fs::path path_to_agent_folder) const;

    private:
        int num_bins;
        const HaxBall haxball;
};

class QuantAngleBallToPlayer : public Quant{
    public:
        QuantAngleBallToPlayer(int num_bins);
        QuantAngleBallToPlayer(fs::path path_to_agent_folder);
        int getQuantisation(const Eigen::Ref<const Eigen::VectorXd>& state) const;
        int getStateSize() const;
        std::string getType() const;
        void saveConfiguration(fs::path path_to_agent_folder) const;

    private:
        int num_bins;
        const HaxBall haxball;
};

class QuantAngleBallToGoal : public Quant{
    public:
        QuantAngleBallToGoal(int num_bins);
        QuantAngleBallToGoal(fs::path path_to_agent_folder);
        int getQuantisation(const Eigen::Ref<const Eigen::VectorXd>& state) const;
        int getStateSize() const;
        std::string getType() const;
        void saveConfiguration(fs::path path_to_agent_folder) const;

    private:
        int num_bins;
        const HaxBall haxball;
};

class QuantAnglePlayerToGoal : public Quant{
    public:
        QuantAnglePlayerToGoal(int num_bins);
        QuantAnglePlayerToGoal(fs::path path_to_agent_folder);
        int getQuantisation(const Eigen::Ref<const Eigen::VectorXd>& state) const;
        int getStateSize() const;
        std::string getType() const;
        void saveConfiguration(fs::path path_to_agent_folder) const;

    private:
        int num_bins;
        const HaxBall haxball;
};

class QuantDistanceBallToGoal : public Quant{
    public:
        QuantDistanceBallToGoal(std::vector<double> bin_borders_);
        QuantDistanceBallToGoal(fs::path path_to_agent_folder);
        int getQuantisation(const Eigen::Ref<const Eigen::VectorXd>& state) const;
        int getStateSize() const;
        std::string getType() const;
        void saveConfiguration(fs::path path_to_agent_folder) const;

    private:
        std::vector<double> bin_borders;
        const HaxBall haxball;
};

class QuantDistanceBallToPlayer : public Quant{
    public:
        QuantDistanceBallToPlayer(std::vector<double> bin_borders_);
        QuantDistanceBallToPlayer(fs::path path_to_agent_folder);
        int getQuantisation(const Eigen::Ref<const Eigen::VectorXd>& state) const;
        int getStateSize() const;
        std::string getType() const;
        void saveConfiguration(fs::path path_to_agent_folder) const;

    private:
        std::vector<double> bin_borders;
        const HaxBall haxball;
};

class QuantBallBehindAgent : public Quant{
    public:
        QuantBallBehindAgent();
        QuantBallBehindAgent(fs::path path_to_agent_folder);
        int getQuantisation(const Eigen::Ref<const Eigen::VectorXd>& state) const;
        int getStateSize() const;
        std::string getType() const;
        void saveConfiguration(fs::path path_to_agent_folder) const;

    private:
        const HaxBall haxball;
};


class QuantAngleBallToPlayerComplex : public Quant{
    public:
        QuantAngleBallToPlayerComplex(int num_angles_forward, int num_angles_backward, bool extra_feature_ball_behind_agent);
        QuantAngleBallToPlayerComplex(fs::path path_to_agent_folder);
        int getQuantisation(const Eigen::Ref<const Eigen::VectorXd>& state) const;
        int getStateSize() const;
        std::string getType() const;
        void saveConfiguration(fs::path path_to_agent_folder) const;

    private:
        int num_angles_forward;
        int num_angles_backward; 
        bool extra_feature_ball_behind_agent;
        const HaxBall haxball;
};


#endif // _QUANT_H_