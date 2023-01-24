cmake_minimum_required(VERSION 3.0.0)

project(HaxBallAgentGroup-F)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -ffast-math")

find_package(Qt5 COMPONENTS Widgets Gui REQUIRED)
find_package(OpenMP REQUIRED)

include_directories(
    HaxBallEnv/
    HaxBallEnv/include
    HaxBallEnv/include/Eigen
    Trainer/include
    Agents/include
    Reward/include
    Greediness/include
    Utilities/include
    Quantisation/include
    libraries/progressbar/include
    )

set(SRC_FILES
    main.cpp
    HaxBallEnv/src/BaseAgent.cpp
    HaxBallEnv/src/RandomAgent.cpp
    HaxBallEnv/src/HaxBall.cpp
    HaxBallEnv/src/HaxBallGui.cpp
    Trainer/trainer.cpp
    Agents/Q_Agent.cpp
    Agents/Q_Agent_2.cpp
    Reward/RewardPoint.cpp
    Reward/RewardBall.cpp
    Reward/BaseReward.cpp
    Reward/RewardGoal.cpp
    Greediness/greediness.cpp
    Utilities/file_handling.cpp
    Utilities/QuantUtils.cpp
    Quantisation/QuantGridPlayer.cpp
    Quantisation/QuantGridPlayerBall.cpp
    Quantisation/QuantGridPlayerAngleBall.cpp
    #Quantisation/Quantisation1.cpp
    Quantisation/Quantisation.cpp
    Quantisation/Quants/QuantVelocity.cpp
    Quantisation/Quants/QuantVelocityX.cpp
    Quantisation/Quants/QuantVelocityY.cpp
    Quantisation/Quants/QuantPlayerPosX.cpp
    Quantisation/Quants/QuantPlayerPosY.cpp
    Quantisation/Quants/QuantBallPosX.cpp
    Quantisation/Quants/QuantBallPosY.cpp
    Quantisation/Quants/QuantAngleBallToPlayer.cpp
    Quantisation/Quants/QuantAngleBallToGoal.cpp
    Quantisation/Quants/QuantAnglePlayerToGoal.cpp
    Quantisation/Quants/QuantDistanceBallToGoal.cpp
    Quantisation/Quants/QuantDistanceBallToPlayer.cpp
    Quantisation/Quants/QuantBallBehindAgent.cpp
    Quantisation/Quants/QuantAngleBallToPlayerComplex.cpp
    )

set(MOC_FILES
    HaxBallEnv/include/HaxBall.h
    HaxBallEnv/include/HaxBallGui.h)

qt5_wrap_cpp(SRC_FILES ${MOC_FILES})

add_executable(${PROJECT_NAME} ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Gui OpenMP::OpenMP_CXX)
