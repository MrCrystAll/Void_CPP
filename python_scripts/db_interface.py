import datetime
import time
from typing import Any, Dict
from pymongo.mongo_client import MongoClient

connection = MongoClient('localhost', 27017)
database = connection.get_database("Void")
metrics_collection = getattr(database, "Metrics")
runs_collection = getattr(database, "Runs")

def send_metrics(name: str, _id: str, metrics: Dict[str, Any]):
    copy_metrics = metrics.copy()
    copy_metrics.setdefault("Run name", name)
    copy_metrics.setdefault("Run ID", _id)
    metrics_collection.insert_one(copy_metrics)
    
def save_run(run_name, run_id, project_name, group_name, _continue: bool = True):
    run = {
        "Run name": run_name,
        "Run ID": run_id,
        "Project name": project_name,
        "Group name": group_name,
        "Created": datetime.datetime.now(),
        "Running": True
    }
    
    if __get_n_runs(run_name, run_id) < 1:
        runs_collection.insert_one(run)
    else:
        runs_collection.update_one({"Run name": run_name, "Run ID": run_id}, {"$set": {"Running": True}})
    
def __get_n_runs(run_name, run_id):
    return runs_collection.count_documents({"Run name": run_name, "Run ID": run_id})

def db_end_run(run_name, run_id):
    runs_collection.update_one({"Run name": run_name, "Run ID": run_id}, {"$set": {"Running": False}})