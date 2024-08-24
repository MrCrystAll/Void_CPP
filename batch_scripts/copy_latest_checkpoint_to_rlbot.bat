@echo off
setlocal enabledelayedexpansion

:: Check if an argument is passed
if "%~1"=="" (
    echo No submodule specified, using the latest folder in checkpoints.
    set "use_latest=true"
) else (
    echo Submodule specified: %~1
    set "submodule=%~1"
    set "use_latest=false"
)

set "source=..\checkpoints"
set "destination=..\Release\RLBotModel"

if "!use_latest!"=="true" (
    :: Find the most recent folder in the root of checkpoints
    for /f "delims=" %%a in ('dir /ad /b /o-d "%source%"') do (
        set "latest=%%a"
        goto :prepareCopy
    )
) else (
    :: Navigate to the specified submodule and find the most recent folder
    set "submoduleDir=%source%\%submodule%"

    if exist "!submoduleDir!" (
        for /f "delims=" %%a in ('dir /ad /b /o-d "!submoduleDir!"') do (
            set "latest=%%a"
            set "source=!submoduleDir!"
            goto :prepareCopy
        )
    ) else (
        echo The specified submodule "%submodule%" does not exist.
        endlocal
        exit /b 1
    )
)

:prepareCopy
:: Delete all existing contents in RLBotModel directory
if exist "%destination%" (
    echo Deleting all contents in "%destination%"...
    rd /s /q "%destination%"
    mkdir "%destination%"
)

:copy
:: Copy the new folder to RLBotModel
echo Copying folder "%source%\!latest!" to "%destination%\!latest!"...
xcopy /s /e /i "%source%\!latest!" "%destination%\!latest!"

endlocal
