@echo off

setlocal ENABLEDELAYEDEXPANSION

set runtime.platform.path=%~dp0
set ctrl_file=rumps_compile_param.txt

rem type "%~dp0rumps_compile_param.txt"
rem for /f "usebackq delims== tokens=1,2" %%G in ("%runtime.platform.path%%ctrl_file%") DO set %%G=%%H

echo %ide.hardware.path%
echo %ide.tools_builder%
echo %ide.tools_avr%
echo %ide.libraries%
echo %ide.build.path%
echo %rumpsduino1_0.hardware.path%
echo %rumpsduino1_0.hardware.path%

rem set /a core_itr+=1

rem echo core_itr=%core_itr%
rem echo %core_itr%>"%runtime.platform.path%%ctrl_file%"

setlocal DISABLEDELAYEDEXPANSION