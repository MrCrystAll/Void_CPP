#include "States.h"

USE_STATES_NS;

RLGSC::GameState WeightedSampleSetter::ResetState(Arena* arena)
{
    //Commented while multi game modes are disabled
    //this->BuildArenaConfig(arena);

    float weightSum = 0;

    for (std::pair p : setters) {
        weightSum += p.second;
    }

    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> distrib(0, weightSum);

    float pick = distrib(gen);

    float current = 0;

    for (std::pair p : setters) {
        current += p.second;
        if (pick > current) {
            continue;
        }
        
        //Chosen setter will be here
        p.first->ResetState(arena);
        break;
    }

    return RLGSC::GameState(arena);
}

void WeightedSampleSetter::BuildArenaConfig(Arena* arena)
{
    //Based on gm probs, choose game mode
    float maxTeamSize = gmProbs.size();

    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(1, maxTeamSize);

    int teamSize = distrib(gen);

    int nBlue = 0, nOrange = 0;
    for (Car* c : arena->GetCars()) {
        if (c->team == Team::BLUE) {
            nBlue++;
        }
        else {
            nOrange++;
        }
    }

    int blueToSpawn = teamSize - nBlue > 0 ? teamSize - nBlue : 0;
    int orangeToSpawn = teamSize - nOrange > 0 ? teamSize - nOrange : 0;

    for (int i = 0; i < blueToSpawn; i++) {
        arena->AddCar(Team::BLUE);
    }

    for (int i = 0; i < orangeToSpawn; i++) {
        arena->AddCar(Team::ORANGE);
    }

    int blueToRemove = teamSize - nBlue < 0 ? nBlue - teamSize : 0;
    int orangeToRemove = teamSize - nOrange < 0 ? nOrange - teamSize : 0;

    for (int i = 0; i < blueToRemove; i++) {
        for (Car* c : arena->GetCars()) {
            if (c->team == Team::BLUE) {
                arena->RemoveCar(c);
            }
        }
    }

    for (int i = 0; i < orangeToRemove; i++) {
        for (Car* c : arena->GetCars()) {
            if (c->team == Team::ORANGE) {
                arena->RemoveCar(c);
            }
        }
    }
}

RLGSC::GameState EmptyBoostKickoffSetter::ResetState(Arena* arena)
{
    arena->ResetToRandomKickoff();
    for (Car* c : arena->GetCars()) {
        CarState cs = c->GetState();
        cs.boost = 0;

        c->SetState(cs);
    }

    return RLGSC::GameState(arena);
}
