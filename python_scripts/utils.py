import json
from math import floor
import os
import sys
from rlgym_ppo.ppo import PPOLearner

CHECKPOINTS_CPP_PATH = "..\\checkpoints\\"
CHECKPOINTS_PY_PATH = "checkpoints\\"

class JsonConfig:
    def __init__(self, cumulative_timesteps: int = 0):
        self.cumulative_timesteps = cumulative_timesteps
    
    def training_time(self, tick_skip: int):
        n_seconds = (tick_skip / 120) * self.cumulative_timesteps
        n_years = n_seconds / (60 * 60 * 24 * 365.25)
        diff_years = n_years - floor(n_years)
        n_years = floor(n_years)

        n_days = diff_years * 365.25
        diff_days = n_days - floor(n_days)
        n_days = floor(n_days)
        
        n_hours = diff_days * 24
        diff_hours = n_hours - floor(n_hours)
        n_hours = floor(n_hours)
        
        n_minutes = diff_hours * 60
        diff_minutes = n_minutes - floor(n_minutes)
        n_minutes = floor(n_minutes)
        
        n_seconds = floor(diff_minutes * 60)
        
        final_path = f"{n_years}y {n_days}d {n_hours}h {n_minutes}m {n_seconds}s" 
        if n_years == 0:
            final_path = f"{n_days}d {n_hours}h {n_minutes}m {n_seconds}s"
            
        return final_path

class AgentConfig:
    def __init__(self, ppo_learner: PPOLearner, json_config: JsonConfig):
        self.ppo_learner = ppo_learner
        self.json_config = json_config

def _predicate(path):
    return os.stat(path).st_mtime

def get_latest_model_path(checkpoints_path: str) -> str:
    latest_run = \
    sorted(os.listdir(checkpoints_path), key=lambda y: _predicate(os.path.join(checkpoints_path, y)), reverse=True)[0]
    return os.path.join(checkpoints_path, latest_run)

def get_latest_cpp_model(cpp_model_path: str = CHECKPOINTS_CPP_PATH, submodel: str = ""):
    return get_latest_model_path(checkpoints_path=cpp_model_path + submodel)

def get_latest_py_model(py_model_path: str = CHECKPOINTS_PY_PATH, submodel: str = ""):
    return get_latest_model_path(checkpoints_path=py_model_path + submodel)


def live_log(*content):
    print("[LIVE INSTANCE] -", *content)
    

def convert_cpp_to_py(path: str) -> str:
    split_path = path.split("\\")
    py_model_path = os.path.join(CHECKPOINTS_PY_PATH, split_path[-2], split_path[-1])
    os.system(f"{sys.executable} \"../tools/checkpoint_converter.py\" to_python " + path + " " + py_model_path)
    return py_model_path
    
def load_latest_checkpoint(ppo_learner: PPOLearner, submodel: str) -> AgentConfig:
    
    agent_config = AgentConfig(ppo_learner=ppo_learner, json_config=JsonConfig())
    
    latest_cpp = get_latest_cpp_model(submodel=submodel)
    py_model_path = convert_cpp_to_py(latest_cpp)
    
    latest_cpp_json = os.path.join(latest_cpp, "RUNNING_STATS.json")
    
    with open(latest_cpp_json, "r") as f:
        data = json.load(f)
        agent_config.json_config.cumulative_timesteps = data["cumulative_timesteps"]
        
    ppo_learner.load_from(py_model_path)
    