@echo off

set "ReleaseDir=%cd%\..\Release"
set "TestDir=%cd%\..\tests"

:: List of folders to link
set folders=submodels checkpoints replays Configs collision_meshes python_scripts

:: Loop through each folder and create a symbolic link
for %%f in (%folders%) do (
    if not exist "%ReleaseDir%\%%f" (
        mklink /J "%ReleaseDir%\%%f" "%cd%\..\%%f"
        echo Created symbolic link in %ReleaseDir% for %%f
    ) else (
        echo Link for %%f already exists
    )
	
	if not exist "%TestDir%\%%f" (
        mklink /J "%TestDir%\%%f" "%cd%\..\%%f"
        echo Created symbolic link in %TestDir% for %%f
    ) else (
        echo Link for %%f already exists
    )
)

PAUSE