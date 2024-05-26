import random
from typing import Tuple
from rlgym_sim.utils.state_setters.state_setter import StateSetter, StateWrapper
from rlgym_tools.extra_state_setters.weighted_sample_setter import WeightedSampleSetter

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