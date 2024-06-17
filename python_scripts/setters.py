from math import pi
import random
from typing import Tuple
import numpy as np
from rlgym_sim.utils.state_setters.state_setter import StateSetter, StateWrapper
from rlgym_tools.extra_state_setters.weighted_sample_setter import WeightedSampleSetter
from rlgym_sim.utils.common_values import BALL_RADIUS

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
        state_wrapper.cars[0].boost = 100
        
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
        state_wrapper.cars[0].boost = 100
        
        return state_wrapper
    
    
class OverfitCeilingPinchSetter(StateSetter):
    def reset(self, state_wrapper: StateWrapper):
        
        state_wrapper.ball.set_pos(-2100, -3000, BALL_RADIUS)
        state_wrapper.ball.set_lin_vel(-150, 100, 10)
        
        state_wrapper.cars[0].set_pos(-1800, -3000, 17)
        state_wrapper.cars[0].set_rot(yaw=np.pi)
        state_wrapper.cars[0].boost = 1
        
        return state_wrapper