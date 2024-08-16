@echo off

set "ReleaseDir=%cd%\..\Release"

:: List of folders to link
set folders=submodels checkpoints replays Configs collision_meshes python_scripts

:: Loop through each folder and create a symbolic link
for %%f in (%folders%) do (
    if not exist "%ReleaseDir%\%%f" (
        mklink /J "%ReleaseDir%\%%f" "%cd%\..\%%f"
        echo Created symbolic link for %%f
    ) else (
        echo Link for %%f already exists
    )
)

PAUSE