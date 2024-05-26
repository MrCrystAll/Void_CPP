import os


def _predicate(path):
    return os.stat(path).st_mtime


def get_latest_model_path(checkpoints_path: str) -> str:
    latest_run = \
    sorted(os.listdir(checkpoints_path), key=lambda y: _predicate(os.path.join(checkpoints_path, y)), reverse=True)[0]
    latest_model = sorted(os.listdir(os.path.join(checkpoints_path, latest_run)),
                          key=lambda y: _predicate(os.path.join(checkpoints_path, latest_run, y)), reverse=True)[0]
    return os.path.join(checkpoints_path, latest_run, latest_model)


def live_log(*content):
    print("[LIVE INSTANCE] -", *content)
