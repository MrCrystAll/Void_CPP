#include <Replays/ReplayUtils.h>
#include <__msvc_chrono.hpp>
#include <vector>
#include <RLGymSim_CPP/Utils/BasicTypes/Lists.h>
#include <RLGymSim_CPP/Utils/CommonValues.h>

START_REPLAY_NS;

std::string GetTimePath()
{
	auto end = std::chrono::system_clock::now();
	std::time_t time = std::chrono::system_clock::to_time_t(end);
	std::tm* localTime = std::localtime(&time);
	return std::to_string(localTime->tm_mday) + "_" + std::to_string(localTime->tm_hour) + "_" + std::to_string(localTime->tm_min) + "_" + std::to_string(localTime->tm_sec);
}

#define T_r -36.07956616966136  // torque coefficient for roll
#define T_p -12.14599781908070  // torque coefficient for pitch
#define T_y 8.91962804287785  // torque coefficient for yaw
#define D_r -4.47166302201591  // drag coefficient for roll
#define D_p -2.798194258050845  // drag coefficient for pitch
#define D_y -1.886491900437232  // drag coefficient for yaw

using namespace RLGSC::CommonValues;

std::vector<float> AerialInputs(Vec angVelStart, Vec angVelEnd, RotMat rotMatStart, RotMat rotMatEnd, float dt, bool isFlipping) {
    float scale = 1.0;
    if (angVelEnd.Length() >= CAR_MAX_ANG_VEL - 0.01) {
        scale = 1.25;  // Scale up so we don't get partial inputs when we hit the max angular velocity
    }

    Vec tau = (angVelEnd * scale - angVelStart) / dt;  // net torque in world coordinates
    RotMat tst = rotMatStart.Transpose();
    tau = Vec(
        tst[0][0] * tau[0] + tst[0][1] * tau[1] + tst[0][2] * tau[2],
        tst[1][0] * tau[0] + tst[1][1] * tau[1] + tst[1][2] * tau[2],
        tst[2][0] * tau[0] + tst[2][1] * tau[1] + tst[2][2] * tau[2]
    );
    Vec ang_vel_local_start = Vec(
        tst[0][0] * angVelStart[0] + tst[0][1] * angVelStart[1] + tst[0][2] * angVelStart[2],
        tst[1][0] * angVelStart[0] + tst[1][1] * angVelStart[1] + tst[1][2] * angVelStart[2],
        tst[2][0] * angVelStart[0] + tst[2][1] * angVelStart[1] + tst[2][2] * angVelStart[2]
    );
    Vec rhs = Vec(
        tau[0] - D_r * ang_vel_local_start[0],
        tau[1] - D_p * ang_vel_local_start[1],
        tau[2] - D_y * ang_vel_local_start[2]
    );

    Vec u = Vec(
        rhs[0] / T_r, // roll
        rhs[1] / (T_p + (0 < rhs[1] - rhs[1] < 0) * ang_vel_local_start[1] * D_p), // pitch
        rhs[2] / (T_y - (0 < rhs[2] - rhs[2] < 0) * ang_vel_local_start[2] * D_y)  // yaw
    );

    //ensure values are between - 1 and +1
    u.x = std::fmax(u.x, -1.0);
    u.x = std::fmin(u.x, 1.0);

    u.y = std::fmax(u.y, -1.0);
    u.y = std::fmin(u.y, 1.0);

    u.z = std::fmax(u.z, -1.0);
    u.z = std::fmin(u.z, 1.0);

    if (isFlipping) {
        // From https ://github.com/ZealanL/RLCarInputSolver/blob/main/src/AirSolver.cpp
        tst = rotMatEnd.Transpose();
        Vec ang_vel_local_end = Vec(
                    tst[0][0] * angVelEnd[0] + tst[0][1] * angVelEnd[1] + tst[0][2] * angVelEnd[2],
                    tst[1][0] * angVelEnd[0] + tst[1][1] * angVelEnd[1] + tst[1][2] * angVelEnd[2],
                    tst[2][0] * angVelEnd[0] + tst[2][1] * angVelEnd[1] + tst[2][2] * angVelEnd[2]
                );

        if (abs(ang_vel_local_start[1]) > abs(ang_vel_local_end[1])) {
            // Flip cancel
            u[1] = (0 < ang_vel_local_start[1] - ang_vel_local_start[1] < 0);
        }                
    }

    return { u[1], u[2], u[0] }; // pitch, yaw, roll
}

END_REPLAY_NS