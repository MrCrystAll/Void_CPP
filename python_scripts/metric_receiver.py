import signal
import site
import sys
import os

lib_path_from_root = os.path.join(os.getcwd(), ".venv/Lib/site-packages")
lib_path_from_release = os.path.join(os.getcwd(), "../.venv/Lib/site-packages")

if os.path.exists(lib_path_from_release):
    sys.path.append(lib_path_from_release)

if os.path.exists(lib_path_from_root):
    sys.path.append(lib_path_from_root)


def log(*content):
    print("[PYTHON] -", *content, flush=True)
    
def err_log(*content):
    log("[ERROR]", content)

wandb_run = None

def log(*content):
    print("[PYTHON]:", *content, flush=True)

sys.path.append(os.path.join(os.getcwd(), "../python_scripts"))
sys.path.append(os.path.join(os.getcwd(), "./python_scripts"))

# Takes in the python executable path, the three wandb init strings, and optionally the current run ID
# Returns the ID of the run (either newly created or resumed)
def init(py_exec_path, project, group, name, id = None):
	"""Takes in the python executable path, the three wandb init strings, and optionally the current run ID. Returns the ID of the run (either newly created or resumed)

	Args:
		py_exec_path (str): Python executable path, necessary to fix a bug where the wrong interpreter is used
		project (str): Wandb project name
		group (str): Wandb group name
		name (str): Wandb run name
		id (str, optional): Id of the wandb run, if None, a new run is created

	Raises:
		Exception: Failed to import wandb

	Returns:
		str: The id of the created or continued run
	"""

	global wandb_run
	
	# Fix the path of our interpreter so wandb doesn't run RLGym_PPO instead of Python
	# Very strange fix for a very strange problem
	sys.executable = py_exec_path
	
	try:
		site_packages_dir = os.path.join(os.path.join(os.path.dirname(py_exec_path), "Lib"), "site-packages")
		site_packages_dir_venv = os.path.join(os.path.join(os.path.dirname(py_exec_path), "../Lib"), "site-packages")
		site_packages_dir_venv_2 = os.path.join(os.path.join(os.path.dirname(py_exec_path), ".venv/Lib"), "site-packages")
		sys.path.append(site_packages_dir)
		site.addsitedir(site_packages_dir_venv)
		site.addsitedir(site_packages_dir_venv_2)

		from db_interface import save_run
		import wandb
	except Exception as e:
		raise Exception(f"""
			FAILED to import wandb! Make sure RLGymPPO_CPP isn't using the wrong Python installation.
			This installation's site packages: {site.getsitepackages()}
			Exception: {repr(e)}"""
		)
	
	log("Calling wandb.init()...")
	if not (id is None) and len(id) > 0:
		wandb_run = wandb.init(project = project, group = group, name = name, id = id, resume = "allow")
	else:
		wandb_run = wandb.init(project = project, group = group, name = name)
  
	save_run(name, wandb_run.id, project, group, id is not None)

	return wandb_run.id

def add_metrics(metrics):
	"""Logs metrics to the wandb run

	Args:
		metrics (Dict[str, Any]): The metrics to log
	"""
	global wandb_run
	from db_interface import send_metrics
	send_metrics(wandb_run.name, wandb_run.id, metrics)
	wandb_run.log(metrics)


def end(_signal):
	"""Runs post-mortem tasks

	Args:
		signal (int): Received signal
	"""
	log(f"Received signal {_signal}, running post-mortem tasks")

	# SIGBREAK crashes wandb_run.finish on a WinError[10054].

	if _signal != signal.Signals.SIGBREAK.value:
		wandb_run.finish()

	from db_interface import db_end_run
	log(f"Ending run {wandb_run.id}")
	db_end_run(wandb_run.id)
