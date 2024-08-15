@echo off
setlocal enabledelayedexpansion

set "source=..\checkpoints"
set "destination=..\Release\RLBotModel"

for /f "delims=" %%a in ('dir /ad /b /o-d "%source%"') do (
    set "latest=%%a"
    goto :copy
)

:copy
xcopy /s /e /i "%source%\%latest%" "%destination%"

endlocal