@echo off
rem this script should be in pre-sketch hooks

setlocal ENABLEDELAYEDEXPANSION

set runtime.platform.path=%~dp0
set counter_filename=%~dsp0..\rumps_runlog.txt
set timer_filename=%~dsp0..\rumps_exectime.txt
set start_time=%TIME%

if exist "%counter_filename%" (
    for /f "usebackq delims== tokens=1,2" %%G in ("%counter_filename%") DO set %%G=%%H
    set "core_count=!core_count: =!"
) else (
    set core_count=0
    echo core_count=!core_count!> "%counter_filename%"
    echo start_time=%start_time%> "%timer_filename%"
)


echo Precompile_script: Compiling for core-!core_count!
echo LALAL %runtime.platform.path%
xcopy "%runtime.platform.path%cores\rumps401\main!core_count!.cpp" "%runtime.platform.path%cores\rumps401\main.cpp" /Y
echo Error Level %ERRORLEVEL%
if %ERRORLEVEL% EQU 0 (
    echo BERHASIL HARUSNYA
)

setlocal DISABLEDELAYEDEXPANSION