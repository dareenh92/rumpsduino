@echo off
rem this script should be post-objcopy
setlocal ENABLEDELAYEDEXPANSION

set runtime.platform.path=%~dp0
set param_filename=%~dsp0..\rumps_compile_param.txt
set counter_filename=%~dsp0..\rumps_runlog.txt
set timer_filename=%~dsp0..\rumps_exectime.txt


if exist "%param_filename%" (
    for /f "usebackq delims== tokens=1,2" %%G in ("%param_filename%") DO set %%G=%%H
    echo Postcompile_script: Compile parameter file successfully loaded 
) else (
    echo Postcompile_script: ERROR rumps_compile_param.txt not found
)

if exist "%counter_filename%" (

    for /f "usebackq delims== tokens=1,2" %%G in ("%counter_filename%") DO set %%G=%%H
    set "core_count=!core_count: =!"
    echo Postcompile_script: Done Compiling core-!core_count!
    if exist "!ide.build.path!\core\main.cpp.o" (
        del !ide.build.path!\core\main.cpp.o
    )

    REM copy "!ide.build.path!\!source.name!.bin" "!runtime.platform.path!\main!core_count!.bin" /Y
    
	if !core_count! LSS 3 (
        set /a core_count+=1

        echo Postcompile_script: Continue compilation for core-!core_count!
        echo core_count=!core_count!> "%counter_filename%"

        "!arduino.builder.path!\arduino-builder.exe" ^
            -compile ^
            -hardware "!ide.hardware.path!" ^
            -hardware "!local.arduino.packages!" ^
            -hardware "!rumpsduino1_0.hardware.path!" ^
            -tools "!ide.tools.builder!" ^
            -tools "!ide.tools.avr!" ^
            -tools "!local.arduino.packages!" ^
            -built-in-libraries "!ide.libraries!" ^
            -libraries "!sketchbook.libraries.path!" ^
            -fqbn=!build.fqbn! ^
            -ide-version=!ide.version! ^
			-build-path "!ide.build.path!" ^
            -warnings="!compiler.warning_flags!" ^
            -prefs=build.warn_data_percentage=!build.warn_data_percentage! ^
            -verbose ^
            "!source.path!\!source.name!"

    ) else (
        echo Postcompile_script: Compilation for 4 cores is done, removing temporary files

        for /L %%G in (0,1,3) DO (
            move /Y "!runtime.platform.path!\main"%%G".bin" "!ide.build.path!\main"%%G".bin"
        )

        if exist "%timer_filename%" (
            for /f "usebackq delims== tokens=1,2" %%G in ("%timer_filename%") DO set %%G=%%H
            set "start_time=!start_time: =!"
            for /F "tokens=1-4 delims=:.," %%a IN ("!start_time!") DO (
                set /A "start=(((%%a*60)+1%%b-100)*60+1%%c-100)*1000+1%%d-100"
            )
            echo Mulai: !start_time!
            set end_time=%TIME%
            for /F "tokens=1-4 delims=:.," %%a IN ("!end_time!") DO (
                set /A "end=(((%%a*60)+1%%b-100)*60+1%%c-100)*1000+1%%d-100"
            )
            set /A "exec=(!end!-!start!)"        
            if !exec! LSS 0 (
                set /A "exec=(!exec!+86400000)%%86400000"
            )
            echo Selesai: !end_time!
            echo Exec_time: !exec! ms
       )

        del %counter_filename% %param_filename% %timer_filename%
         
    )
) else (
    echo Postcompile_script: ERROR rumps_runlog.txt not found
)

setlocal DISABLEDELAYEDEXPANSION