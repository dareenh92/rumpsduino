@echo off

setlocal ENABLEDELAYEDEXPANSION

set runtime.platform.path=%~dp0
set ctrl_file=rumps_compile_param.txt

REM echo file is at : %runtime.platform.path%%ctrl_file%
for /f "usebackq delims== tokens=1,2" %%G in ("%runtime.platform.path%%ctrl_file%") DO set %%G=%%H

set /P core_itr=<"%runtime.platform.path%%ctrl_file%"
set "core_itr=%core_itr: =%"
echo core_itr=%core_itr%

copy "%runtime.platform.path%cores\rumps401\main%core_itr%.cpp" "%runtime.platform.path%cores\rumps401\main.cpp" /Y

setlocal DISABLEDELAYEDEXPANSION