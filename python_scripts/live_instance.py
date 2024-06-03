# region ========================= Imports =========================
import os
import random
import time

import numpy as np
import torch
from rlgym.gamelaunch import LaunchPreference
from rlgym_ppo.ppo import PPOLearner
from rlgym_sim.utils.reward_functions.common_rewards.misc_rewards import ConstantReward
from rlgym_sim.utils.terminal_conditions.common_conditions import TimeoutCondition, GoalScoredCondition
from parsers import NectoAction
from obsBuilders import DefaultObsCpp

from setters import RandomPinchSetter, TeamSizeSetter, PinchSetter
from utils import get_latest_model_path, live_log

# endregion
# region ========================= Environment settings ============================
tick_skip = 8
STEP_TIME = tick_skip / 120.

spawn_opponents = False
blue_count = 1
orange_count = 3 if spawn_opponents else 0

state_mutator = TeamSizeSetter(
    setters=(
        RandomPinchSetter(400, 200, 0.5),
        # dynamic_replay
    ),
    weights=(1,),
    gm_probs=(1, 0, 0)
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
obs_builder = DefaultObsCpp()

agent = PPOLearner(
    obs_space_size=70,
    act_space_size=action_parser.get_action_space().n,
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

model_to_load = "checkpoints/pinchv9"

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

    agent.load_from(model_to_load)

    env = create_env(sim=False)
    current_time = time.time()
    refresh_time = current_time
    rewards = []

    while True:
        playstyle_switch()
        obs = env.reset()
        if len(rewards) > 0:
            last_ep_reward = sum(rewards) / len(rewards)
        rewards = []
        terminated = False
        while not terminated:
            if time.time() - refresh_time >= 1:
                refresh_time = time.time()
                # print_live_state()

            with torch.no_grad():
                actions = np.array([agent.policy.get_action(obs, deterministic=deterministic)[0]])
                actions = actions.reshape((*actions.shape, 1))

            obs, reward, terminated, info = env.step(actions)
            rewards.append(reward)

            if time.time() - current_time >= time_before_update:
                model_reload()
                
            time.sleep(STEP_TIME)

# endregion