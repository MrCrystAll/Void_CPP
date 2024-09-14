import math
from math import pi
import gym
import gym.spaces
import numpy as np
from typing import Any, List
from rlgym_sim.utils import common_values
from rlgym_sim.utils.common_values import CAR_MAX_ANG_VEL, CAR_MAX_SPEED
from rlgym_sim.utils.gamestates import PlayerData, GameState
from rlgym_sim.utils.obs_builders import ObsBuilder

class OnesObs(ObsBuilder):
    def reset(self, initial_state: GameState):
        pass
    
    def build_obs(self, player: PlayerData, state: GameState, previous_action: np.ndarray) -> Any:
        return [1 for _ in range(70)]

class SwappedDefaultObsCpp(ObsBuilder):
    def __init__(self, ang_coef=1/math.pi, lin_vel_coef=1/float(common_values.CAR_MAX_SPEED), ang_vel_coef=1/float(common_values.CAR_MAX_ANG_VEL)):
        """
        :param pos_coef: Position normalization coefficient
        :param ang_coef: Rotation angle normalization coefficient
        :param lin_vel_coef: Linear velocity normalization coefficient
        :param ang_vel_coef: Angular velocity normalization coefficient
        """
        super().__init__()
        self.POS_COEF = np.array([1 / float(common_values.SIDE_WALL_X), 1 / float(common_values.BACK_WALL_Y), 1 / float(common_values.CEILING_Z)])
        self.ANG_COEF = ang_coef
        self.LIN_VEL_COEF = lin_vel_coef
        self.ANG_VEL_COEF = ang_vel_coef

    def reset(self, initial_state: GameState):
        pass

    def build_obs(self, player: PlayerData, state: GameState, previous_action: np.ndarray) -> Any:
        if player.team_num == common_values.ORANGE_TEAM:
            inverted = True
            ball = state.inverted_ball
            pads = state.inverted_boost_pads
        else:
            inverted = False
            ball = state.ball
            pads = state.boost_pads
            
        boost = previous_action[5]
        previous_action[5] = previous_action[6]
        previous_action[6] = boost

        obs = [ball.position * self.POS_COEF,
               ball.linear_velocity * self.LIN_VEL_COEF,
               ball.angular_velocity * self.ANG_VEL_COEF,
               previous_action,
               pads]

        self._add_player_to_obs(obs, player, inverted)

        allies = []
        enemies = []

        for other in state.players:
            if other.car_id == player.car_id:
                continue

            if other.team_num == player.team_num:
                team_obs = allies
            else:
                team_obs = enemies

            self._add_player_to_obs(team_obs, other, inverted)

        obs.extend(allies)
        obs.extend(enemies)
        return np.concatenate(obs)

    def _add_player_to_obs(self, obs: List, player: PlayerData, inverted: bool):
        if inverted:
            player_car = player.inverted_car_data
        else:
            player_car = player.car_data

        obs.extend([
            player_car.position * self.POS_COEF,
            player_car.forward(),
            player_car.up(),
            player_car.linear_velocity * self.LIN_VEL_COEF,
            player_car.angular_velocity * self.ANG_VEL_COEF,
            [player.boost_amount,
             int(player.on_ground),
             int(player.has_flip),
             int(player.is_demoed)]])

        return player_car

class DefaultObsCpp(ObsBuilder):
    def __init__(self, ang_coef=1/math.pi, lin_vel_coef=1/float(common_values.CAR_MAX_SPEED), ang_vel_coef=1/float(common_values.CAR_MAX_ANG_VEL)):
        """
        :param pos_coef: Position normalization coefficient
        :param ang_coef: Rotation angle normalization coefficient
        :param lin_vel_coef: Linear velocity normalization coefficient
        :param ang_vel_coef: Angular velocity normalization coefficient
        """
        super().__init__()
        self.POS_COEF = np.array([1 / float(common_values.SIDE_WALL_X), 1 / float(common_values.BACK_WALL_Y), 1 / float(common_values.CEILING_Z)])
        self.ANG_COEF = ang_coef
        self.LIN_VEL_COEF = lin_vel_coef
        self.ANG_VEL_COEF = ang_vel_coef

    def reset(self, initial_state: GameState):
        pass

    def build_obs(self, player: PlayerData, state: GameState, previous_action: np.ndarray) -> Any:
        if player.team_num == common_values.ORANGE_TEAM:
            inverted = True
            ball = state.inverted_ball
            pads = state.inverted_boost_pads
        else:
            inverted = False
            ball = state.ball
            pads = state.boost_pads

        obs = [ball.position * self.POS_COEF,
               ball.linear_velocity * self.LIN_VEL_COEF,
               ball.angular_velocity * self.ANG_VEL_COEF,
               previous_action,
               pads]

        self._add_player_to_obs(obs, player, inverted)

        allies = []
        enemies = []

        for other in state.players:
            if other.car_id == player.car_id:
                continue

            if other.team_num == player.team_num:
                team_obs = allies
            else:
                team_obs = enemies

            self._add_player_to_obs(team_obs, other, inverted)

        obs.extend(allies)
        obs.extend(enemies)
        return np.concatenate(obs)

    def _add_player_to_obs(self, obs: List, player: PlayerData, inverted: bool):
        if inverted:
            player_car = player.inverted_car_data
        else:
            player_car = player.car_data

        obs.extend([
            player_car.position * self.POS_COEF,
            player_car.forward(),
            player_car.up(),
            player_car.linear_velocity * self.LIN_VEL_COEF,
            player_car.angular_velocity * self.ANG_VEL_COEF,
            [player.boost_amount,
             int(player.on_ground),
             int(player.has_flip),
             int(player.is_demoed)]])

        return player_car
    
    def get_obs_space(self) -> gym.spaces.Space:
        return gym.spaces.Box(-np.inf, np.inf, (70, ))
    
    
class DefaultObsCppPadded(DefaultObsCpp):
    PLAYER_SIZE: int = 19
    
    def __init__(self, ang_coef=1 / math.pi, lin_vel_coef=1 / float(common_values.CAR_MAX_SPEED), ang_vel_coef=1 / float(common_values.CAR_MAX_ANG_VEL), team_size: int = 3, spawn_oppoonents: bool = True):
        super().__init__(ang_coef, lin_vel_coef, ang_vel_coef)
        self.team_size = team_size
        self.spawn_opponents = spawn_oppoonents

        
    def build_obs(self, player: PlayerData, state: GameState, previous_action: np.ndarray) -> Any:
        obs = super().build_obs(player, state, previous_action)
        
        n_to_pad = self.team_size * (self.spawn_opponents + 1) - len(state.players)
        
        for _ in range(n_to_pad):
            obs = self._add_empty_player(obs)
            
        return obs 
    
    def get_obs_space(self) -> gym.spaces.Space:
        return gym.spaces.Box(-np.inf, np.inf, ((70 - self.PLAYER_SIZE) + self.team_size * (self.spawn_opponents + 1) * self.PLAYER_SIZE, ))
        
    def _add_empty_player(self, obs: np.ndarray) -> np.ndarray:
        return np.concatenate((obs, np.zeros(shape=(self.PLAYER_SIZE, ), dtype=np.float32)))

class SwappedDefaultObsCppPadded(SwappedDefaultObsCpp):
    PLAYER_SIZE: int = 19
    
    def __init__(self, ang_coef=1 / math.pi, lin_vel_coef=1 / float(common_values.CAR_MAX_SPEED), ang_vel_coef=1 / float(common_values.CAR_MAX_ANG_VEL), team_size: int = 3, spawn_oppoonents: bool = True):
        super().__init__(ang_coef, lin_vel_coef, ang_vel_coef)
        self.team_size = team_size
        self.spawn_opponents = spawn_oppoonents

        
    def build_obs(self, player: PlayerData, state: GameState, previous_action: np.ndarray) -> Any:
        obs = super().build_obs(player, state, previous_action)
        
        n_to_pad = self.team_size * (self.spawn_opponents + 1) - len(state.players)
        
        for _ in range(n_to_pad):
            obs = self._add_empty_player(obs)
            
        return obs 
    
    def get_obs_space(self) -> gym.spaces.Space:
        return gym.spaces.Box(-np.inf, np.inf, (70 + self.team_size * (self.spawn_opponents + 1) * self.PLAYER_SIZE, ))
        
    def _add_empty_player(self, obs: np.ndarray) -> np.ndarray:
        return np.concatenate((obs, np.zeros(shape=(self.PLAYER_SIZE, ), dtype=np.float32)))