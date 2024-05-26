import sys

import wandb

from typing import Dict, Any

def log(*content):
    print("[PYTHON] -", *content)


wandb_run = None

def format_to_wandb(data: Dict[str, Any], new_data: Dict, trace: str = ""):
    for k, v in data.items():
        if isinstance(v, Dict):
            format_to_wandb(v, new_data, trace + k + "/")
        else:
            new_data.setdefault(trace + k, v)


# Takes in the python executable path, the three wandb init strings, and optionally the current run ID
# Returns the ID of the run (either newly created or resumed)
def init(py_exec_path, project, group, name, id=None):
    global wandb_run

    # Fix the path of our interpreter so wandb doesn't run RLGym_PPO instead of Python
    # Very strange fix for a very strange problem
    sys.executable = py_exec_path

    if wandb_run:
        log(f"Run already exists, returning {wandb_run.id}")
        return wandb_run.id

    if not (id is None) and len(id) > 0:
        wandb_run = wandb.init(project=project, group=group, name=name, id=id, resume="allow")
        log(f"Continuing run {wandb_run.id}")
    else:
        wandb_run = wandb.init(project=project, group=group, name=name)
        log(f"Creating run {wandb_run.id}")

    return wandb_run.id


def add_metrics(metrics):
    global wandb_run
    new_metrics = {}
    format_to_wandb(metrics, new_metrics)
    wandb_run.log(new_metrics)
