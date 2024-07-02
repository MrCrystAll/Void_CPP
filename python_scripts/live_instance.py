# region ========================= Imports =========================
import os
import random
import time
from typing import Dict

import numpy as np
import torch
from rlgym.gamelaunch import LaunchPreference
from rlgym_ppo.ppo import PPOLearner
from rlgym_sim.utils.reward_functions.common_rewards.misc_rewards import ConstantReward
from rlgym_sim.utils.terminal_conditions.common_conditions import TimeoutCondition, GoalScoredCondition
from rlgym_sim.utils.state_setters.default_state import DefaultState
from rlgym_sim.utils.common_values import BLUE_TEAM, ORANGE_TEAM
from rlgym_sim.utils.gamestates.game_state import GameState
from parsers import NectoAction, SwappedNectoAction
from obsBuilders import DefaultObsCpp, DefaultObsCppPadded, OnesObs, SwappedDefaultObsCpp
from rlgym_tools.extra_action_parsers.lookup_act import LookupAction

from setters import OverfitCeilingPinchSetter, RandomPinchSetter, TeamSizeSetter, PinchSetter, DoubleTapSetter
from utils import get_latest_model_path, live_log

# endregion
# region ========================= Environment settings ============================
tick_skip = 8
STEP_TIME = tick_skip / 120.

spawn_opponents = True
blue_count = 1
orange_count = 1 if spawn_opponents else 0

double_tap_args = DoubleTapSetter.DoubleTapSetterArgs(
    both_sides=True,
    ball_variance=DoubleTapSetter.PhysObjVariance(
        vel_variance=np.array([600.0, 1000.0, 100.0])
    ),
    agent_variance=DoubleTapSetter.PhysObjVariance(
        pos_variance=np.array([1000.0, 300.0, 100.0]),
        vel_variance=np.array([100.0, 300.0, 100.0])
    )
)

state_mutator = TeamSizeSetter(
    setters=(
        DoubleTapSetter(double_tap_args),
        # dynamic_replay
    ),
    weights=(1,),
    gm_probs=(1, 1, 1)
)
reward_fn = ConstantReward()

total_timeout = 7
termination_conditions = [
    GoalScoredCondition(),
    TimeoutCondition(int(total_timeout / STEP_TIME)),
]
# endregion
# region ========================= Model Settings =============================

action_parser = NectoAction()
action_size = action_parser.get_action_space().n

obs_builder = DefaultObsCpp()
obs_size = obs_builder.get_obs_space()

if isinstance(obs_size, type(None)):
    raise ValueError("Obs size couldn't be determined, implement \"get_obs_space\" in your observation builder")
else:
    obs_size = obs_size.shape[0]
    
print("=========== Model ============")
print("Observation size:", obs_size)
print("Action size:", action_size)
print("==============================")



agent = PPOLearner(
    obs_space_size=obs_size,
    act_space_size=action_size,
    device="cuda",
    batch_size=10_000,
    mini_batch_size=1_000,
    n_epochs=10,
    continuous_var_range=(0.0, 1.0),
    policy_type=0,
    policy_layer_sizes=(256, 256, 256),
    critic_layer_sizes=(256, 256, 256),
    policy_lr=3e-4,
    critic_lr=3e-4,
    clip_range=0.2,
    ent_coef=0.005)

# endregion
# region ========================= Live instance Settings =============================
deterministic = True

model_to_load = "checkpoints/double_tap/dt_v1"

minutes_before_update = 15
seconds_before_update = 0
time_before_update = minutes_before_update * 60 + seconds_before_update
current_time = 0
last_ep_reward = 0

# endregion
# region ========================= Live instance functions =========================

def create_env(sim: bool = True):
    if sim:
        import rlgym_sim
        rlgym_env = rlgym_sim.make(
            state_setter=state_mutator,
            action_parser=action_parser,
            obs_builder=obs_builder,
            reward_fn=reward_fn,
            terminal_conditions=termination_conditions,
            tick_skip=tick_skip,
            team_size=blue_count,
            spawn_opponents=spawn_opponents,
            # game_speed=1,
            # launch_preference=LaunchPreference.EPIC
        )
    else:
        import rlgym
        rlgym_env = rlgym.make(
            state_setter=state_mutator,
            action_parser=action_parser,
            obs_builder=obs_builder,
            reward_fn=reward_fn,
            terminal_conditions=termination_conditions,
            tick_skip=tick_skip,
            team_size=blue_count,
            spawn_opponents=spawn_opponents,
            game_speed=1,
            launch_preference=LaunchPreference.STEAM
        )

    return rlgym_env


def model_reload():
    global agent, current_time
    latest_model_path = get_latest_model_path("checkpoints")
    agent.load_from(latest_model_path)
    live_log("Model reloaded")
    current_time = time.time()


def playstyle_switch():
    global deterministic

    deterministic = random.uniform(0, 1) > 0.5


def print_live_state():
    ttu = time_before_update - (time.time() - current_time)
    os.system('cls')
    live_log(" === State report === ")
    live_log(f" Mode : {'Deterministic' if deterministic else 'Stochastic'}")
    live_log(f" Model reload in {int(ttu // 60):02d}:{int(ttu % 60):02d}")
    live_log(f" Last episode reward (Average per player) : {last_ep_reward:.6f}")

# endregion
# region ========================= Main loop =========================

if __name__ == "__main__":

    # agent.load_from(model_to_load)
    sim = True
    env = create_env(sim=sim)
    current_time = time.time()
    refresh_time = current_time

    while True:
        playstyle_switch()
        obs, info = env.reset(return_info=True)
        terminated = False
        
        cnt_blue_count = len([0 for p in info["state"].players if p.team_num == BLUE_TEAM ])
        cnt_orange_count = cnt_blue_count if spawn_opponents else 0
        
        print("Number of blue agents: ", cnt_blue_count)
        print("Number of orange agents: ", cnt_orange_count)
        print("Total number of agents: ", cnt_blue_count + cnt_orange_count)
        
        while not terminated:
            if time.time() - refresh_time >= 1:
                refresh_time = time.time()
                # print_live_state()            
            
            if cnt_blue_count + cnt_orange_count == 1:
                obs = [obs]

            with torch.no_grad():
                actions = np.array([agent.policy.get_action(obs[i], deterministic=deterministic)[0] for i in range(cnt_blue_count + cnt_orange_count)])
                actions = actions.reshape((*actions.shape, 1))

            obs, reward, terminated, info = env.step(actions)
            if time.time() - current_time >= time_before_update:
                model_reload()
                
            if sim:
                time.sleep(STEP_TIME)

# endregion