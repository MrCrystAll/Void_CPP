from math import pi
import random
from typing import Tuple
import numpy as np
from rlgym_sim.utils.state_setters.state_setter import StateSetter, StateWrapper
from rlgym_tools.extra_state_setters.weighted_sample_setter import WeightedSampleSetter
from rlgym_sim.utils.common_values import BALL_RADIUS, BLUE_TEAM, ORANGE_TEAM, BACK_WALL_Y

class TeamSizeSetter(StateSetter):
    def __init__(self, gm_probs: Tuple[float, float, float], setters: Tuple[StateSetter, ...],
                 weights: Tuple[float, ...]):
        super().__init__()
        self.gm_probs = gm_probs
        self.state = WeightedSampleSetter(state_setters=setters, weights=weights)
        self.count = 3

    def build_wrapper(self, max_team_size: int, spawn_opponents: bool) -> StateWrapper:
        selected_gm = random.choices(range(1, 4), weights=self.gm_probs, k=1)[0]
        self.count = selected_gm
        wrapper = StateWrapper(blue_count=self.count, orange_count=self.count if spawn_opponents else 0)

        return wrapper

    def reset(self, state_wrapper: StateWrapper):
        self.state.reset(state_wrapper)
        
        
class PinchSetter(StateSetter):
    def reset(self, state_wrapper: StateWrapper):
        state_wrapper.ball.set_pos(-2000, -3200, BALL_RADIUS)
        state_wrapper.ball.set_lin_vel(-1400, 400, 10)
        
        state_wrapper.cars[0].set_pos(-1700, -3200, 20)
        state_wrapper.cars[0].set_lin_vel(-300, 400, 10)
        state_wrapper.cars[0].set_rot(yaw=0.90 * np.pi)
        state_wrapper.cars[0].boost = 1
        
        return state_wrapper
    
class RandomPinchSetter(StateSetter):
    def __init__(self, pos_variance: int, vel_variance: int, orient_variance):
        self.pos_variance = pos_variance
        self.vel_variance = vel_variance
        self.orient_variance = orient_variance
    
    def reset(self, state_wrapper: StateWrapper):
        
        dir = int(random.uniform(0, 1) > 0.5)
        dir = -1 if dir == 0 else 1
        
        orientation = 0.9 * np.pi
        orientation = orientation if dir == -1 else np.pi - orientation
        
        state_wrapper.ball.set_pos((random.randint(0, self.pos_variance) + 2000) * dir, -3000, BALL_RADIUS)
        state_wrapper.ball.set_lin_vel((random.randint(0, self.vel_variance) + 1400) * dir, 400, 10)
        
        state_wrapper.cars[0].set_pos((random.randint(0, self.pos_variance) + 300) * dir * -1 + state_wrapper.ball.position[0], -3200, 20)
        state_wrapper.cars[0].set_lin_vel((random.randint(0, self.vel_variance) + 300) * dir, 400, 10)
        state_wrapper.cars[0].set_rot(yaw=orientation)
        state_wrapper.cars[0].boost = 1
        
        return state_wrapper
    
    
class OverfitCeilingPinchSetter(StateSetter):
    def reset(self, state_wrapper: StateWrapper):
        
        state_wrapper.ball.set_pos(-2100, -3000, BALL_RADIUS)
        state_wrapper.ball.set_lin_vel(-150, 100, 10)
        
        state_wrapper.cars[0].set_pos(-1800, -3000, 17)
        state_wrapper.cars[0].set_rot(yaw=np.pi)
        state_wrapper.cars[0].boost = 1
        
        return state_wrapper
    
def randomVec(vec: np.ndarray):
    return np.array(tuple(random.uniform(0, f) for f in vec))
    
class DoubleTapSetter(StateSetter):
    
    class PhysObjVariance(object):
        def __init__(
            self, 
            pos_variance: np.ndarray = np.array([400.0, 0, 200.0]),
            vel_variance: np.ndarray = np.array([400.0, 400.0, 100.0]),
            ang_vel_variance: np.ndarray = np.array([100.0, 100.0, 100.0]),
            orient_variance: np.ndarray = np.array([2.0, 2.0, 2.0])
        ) -> None:
            self.pos_variance = pos_variance
            self.vel_variance = vel_variance
            self.ang_vel_variance = ang_vel_variance
            self.orient_variance = orient_variance
    
    class DoubleTapSetterArgs(object):
        def __init__(
                self, 
                both_sides: bool,
                ball_variance: "DoubleTapSetter.PhysObjVariance",
                agent_variance: "DoubleTapSetter.PhysObjVariance",
            ) -> None:
            self.both_sides = both_sides
            self.ball_variance = ball_variance
            self.agent_variance = agent_variance
    
    def __init__(self, config: "DoubleTapSetter.DoubleTapSetterArgs") -> None:
        super().__init__()
        self.config = config
    
    def reset(self, state_wrapper: StateWrapper):
        
        chosenTeam = BLUE_TEAM

        if self.config.both_sides:
            chosenTeam = BLUE_TEAM if random.uniform(0, 1) < 0.5 else ORANGE_TEAM

        yDir = 0

        #Setting directions
        if chosenTeam == BLUE_TEAM:
            yDir = 1
        else:
            yDir = -1

        ballInitialPos = (0, 3000 * yDir, 1200)
        agentInitialPos = (0, 600 * yDir, 1100)

        ballInitialVel = (0, 2000 * yDir, 300)
        agentInitialVel = (0, 1750 * yDir, 550)
        agentInitialAngVel = (0, 0, 0)

        agentInitialOrientation = (pi / 2, 0, 0)

        ballInitialPos += randomVec(self.config.ball_variance.pos_variance)
        ballInitialPos[0] -= self.config.ball_variance.pos_variance[0] / 2.0

        ballInitialVel += randomVec(self.config.ball_variance.vel_variance)
        ballInitialVel[0] -= self.config.ball_variance.vel_variance[0] / 2.0

        agentInitialPos += randomVec(self.config.agent_variance.pos_variance)
        agentInitialPos[0] -= self.config.agent_variance.pos_variance[0] / 2.0

        agentInitialVel += randomVec(self.config.agent_variance.vel_variance)
        agentInitialVel[0] -= self.config.agent_variance.vel_variance[0] / 2.0

        agentInitialAngVel += randomVec(self.config.agent_variance.ang_vel_variance)
        agentInitialAngVel[0] -= self.config.agent_variance.ang_vel_variance[0] / 2.0
        agentInitialAngVel[1] -= self.config.agent_variance.ang_vel_variance[1] / 2.0
        agentInitialAngVel[2] -= self.config.agent_variance.ang_vel_variance[2] / 2.0

        agentInitialOrientation = agentInitialOrientation + randomVec(self.config.agent_variance.orient_variance)        
        state_wrapper.ball.set_pos(*ballInitialPos)
        state_wrapper.ball.set_lin_vel(*ballInitialVel)

        for c in state_wrapper.cars:

            if chosenTeam == c.team_num:
                c.set_pos(*agentInitialPos)
                c.set_lin_vel(*agentInitialVel)
                c.set_rot(agentInitialOrientation[1], agentInitialOrientation[0], agentInitialOrientation[2])
                c.set_ang_vel(*agentInitialAngVel)
            
            else:
                c.set_pos(0, BACK_WALL_Y * yDir, 17)

            c.boost = 1

        return state_wrapper
