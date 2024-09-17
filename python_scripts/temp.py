import os
from signal import Signals
import signal
import sys
import time

lib_path_from_root = os.path.join(os.getcwd(), ".venv/Lib/site-packages")
lib_path_from_release = os.path.join(os.getcwd(), "../.venv/Lib/site-packages")

if os.path.exists(lib_path_from_release):
    sys.path.append(lib_path_from_release)

if os.path.exists(lib_path_from_root):
    sys.path.append(lib_path_from_root)
    
lib_pyscript_from_root = os.path.join(os.getcwd(), "python_scripts")
lib_pyscript_from_release = os.path.join(os.getcwd(), "../python_scripts")

if os.path.exists(lib_pyscript_from_root):
    sys.path.append(lib_pyscript_from_root)

if os.path.exists(lib_pyscript_from_release):
    sys.path.append(lib_pyscript_from_release)

import wandb
import atexit
from requests import post
from typing import Dict, Any

use_ui = True
if use_ui:
    from db_interface import send_metrics, save_run, db_end_run


def log(*content):
    print("[PYTHON] -", *content, flush=True)
    
def err_log(*content):
    log("[ERROR]", content)

wandb_run = None
project_run_name = None
project_group = None
project_name = None
project_run_id = None

def format_to_wandb(data: Dict[str, Any], new_data: Dict, trace: str = ""):
    for k, v in data.items():
        if isinstance(v, Dict):
            format_to_wandb(v, new_data, trace + k + "/")
        else:
            new_data.setdefault(trace + k, v)


# Takes in the python executable path, the three wandb init strings, and optionally the current run ID
# Returns the ID of the run (either newly created or resumed)
def init(py_exec_path, project, group, name, id=None):
    global wandb_run, project_name, project_group, project_run_name, project_run_id

    # Fix the path of our interpreter so wandb doesn't run RLGym_PPO instead of Python
    # Very strange fix for a very strange problem
    sys.executable = py_exec_path
    
    project_run_name = name
    project_group = group
    project_name = project
  
    if wandb_run:
        log(f"Run already exists, returning {wandb_run.id}")
        project_run_id = wandb_run.id
        return wandb_run.id

    if not (id is None) and len(id) > 0:
        wandb_run = wandb.init(project=project, group=group, name=name, id=id, resume="allow")
        project_run_id = wandb_run.id
        if use_ui:
            save_run(run_id=project_run_id, run_name=project_run_name, project_name=project_name, group_name=project_group, _continue=True)
        log(f"Continuing run {wandb_run.id}")
    else:
        wandb_run = wandb.init(project=project, group=group, name=name)
        project_run_id = wandb_run.id
        if use_ui:
            save_run(run_id=project_run_id, run_name=project_run_name, project_name=project_name, group_name=project_group, _continue=False)
        log(f"Creating run {wandb_run.id}")
        
    # api_post("wandbRunData", {
    #     "group": group,
    #     "name": name,
    #     "project": project,
    #     "id": wandb_run.id
    # })

    return wandb_run.id

def api_post(context, data):
    try:
        log(f"Posting to the API on context /{context}...")
        post(f"http://localhost:3000/{context}", json=data, headers={"authorization": "validToken"})
        log(f"Successfully posted data {data}")
    except Exception as e:
        err_log(e)
        
def format_from_wandb(key: str, value: Any) -> Dict[str, Any]:
        all_attr = key.split("/")
        data = {}
        temp_ref = data

        len_attrs = 0
        while len_attrs < len(all_attr) - 1:
            temp_ref.setdefault(all_attr[len_attrs], {})
            temp_ref = temp_ref[all_attr[len_attrs]]
            len_attrs += 1

            if len_attrs == len(all_attr) - 1:
                temp_ref[all_attr[-1]] = value

        return data


def add_metrics(metrics):
    global wandb_run
    new_metrics = {}
    format_to_wandb(metrics, new_metrics)
    
    if use_ui:
        send_metrics(project_run_name, project_run_id, new_metrics)
    
    wandb_run.log(new_metrics)


def end(end_signal: int):
    try:
        log(f"Received signal {end_signal} ({signal.strsignal(end_signal)}), running post-mortem tasks")
        if wandb_run and end_signal != Signals.SIGBREAK:
            log("Finishing wandb")
            wandb_run.finish()
            log("Finished wandb run")
            
        if end_signal == Signals.SIGBREAK.value:
            log("Can't finish wandb run with end signal 21, please use Ctrl + C instead of closing the window")
        
        if use_ui:
            db_end_run(project_run_name, project_run_id)
            log("Finished db modifications")
    except Exception as e:
        err_log(e)