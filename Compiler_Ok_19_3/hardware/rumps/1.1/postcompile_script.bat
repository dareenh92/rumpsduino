@echo off
rem this script should be post-objcopy

setlocal ENABLEDELAYEDEXPANSION

set runtime.platform.path=%~dp0
set param_filename=%~dsp0..\rumps_compile_param.txt
set counter_filename=%~dsp0..\rumps_runlog.txt
set timer_filename=%~dsp0..\rumps_exectime.txt

rem type "%param_filename%"
if exist "%param_filename%" (
    for /f "usebackq delims== tokens=1,2" %%G in ("%param_filename%") DO set %%G=%%H
    echo Postcompile_script: Compile parameter file successfully loaded 
    REM echo !ide.hardware.path!
    REM echo !ide.tools.builder!
    REM echo !ide.tools.avr!
    REM echo !ide.libraries!
    REM echo !ide.build.path!
    REM echo !rumpsduino1_0.hardware.path!
    REM echo !sketchbook.libraries.path!
    REM echo !source.path!
    REM echo !source.name!
    REM echo !build.fqbn!
    REM echo !ide.version!
    REM echo !build.warn_data_percentage!
	REM echo !runtime.platform.path!
) else (
    echo Postcompile_script: ERROR rumps_compile_param.txt not found
)

if exist "%counter_filename%" (
    for /f "usebackq delims== tokens=1,2" %%G in ("%counter_filename%") DO set %%G=%%H
    set "core_count=!core_count: =!"
    echo Postcompile_script: Done Compiling core-!core_count!
    
    rem save binary as main#.bin
    copy "!ide.build.path!\!source.name!.bin" "!runtime.platform.path!\main!core_count!.bin" /Y

    rem if !core_count! LSS 3 (
	if !core_count! LSS 3 (
        rem less than 3, core_count++ AND call builder

        set /a core_count+=1

        echo Postcompile_script: Continue compilation for core-!core_count!
        echo core_count=!core_count!> "%counter_filename%"

        REM "!ide.hardware.path!\..\arduino-builder.exe" ^
        REM     -compile ^
        REM     -logger=machine ^
        REM     -hardware "!ide.hardware.path!" ^
        REM     -hardware "!rumpsduino1_0.hardware.path!" ^
        REM     -tools "!ide.tools.builder!" ^
        REM     -tools "!ide.tools.avr!" ^
        REM     -built-in-libraries "!ide.libraries!" ^
        REM     -libraries "!sketchbook.libraries.path!" ^
        REM     -build-path "!ide.build.path!" ^
        REM     -fqbn=!build.fqbn! ^
        REM     -ide-version=!ide.version! ^
        REM     -prefs=build.warn_data_percentage=!build.warn_data_percentage! ^
        REM     -warnings=default ^
        REM     -verbose ^
        REM     "!source.path!\!source.name!"

        REM Default Value dari Builder
        rem -warnings="!compiler.warning_flags!" ^
        rem -build-cache "!includes.cache!" ^
        rem -prefs=build.warn_data_percentage=!build.warn_data_percentage! ^
        rem -hardware "!local.arduino.packages!" ^
        rem -tools "!local.arduino.packages!" ^
        rem C:\Users\Stefanus K\AppData\Local\Arduino15\packages
    echo Packages = !local.arduino.packages!
        "!ide.hardware.path!\..\arduino-builder.exe" ^
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
            -prefs=build.warn_data_percentage=!build.warn_data_percentage! ^
            -verbose ^
            "!source.path!\!source.name!"

    ) else (
        rem more than equal 3, re-copy bin files, delete param & counter files

        echo Postcompile_script: Compilation for 4 cores is done, removing temporary files

        rem copy main#.bin back to build.path
        move /Y "!runtime.platform.path!\main0.bin" "!ide.build.path!\main0.bin"
        move /Y "!runtime.platform.path!\main1.bin" "!ide.build.path!\main1.bin"
        move /Y "!runtime.platform.path!\main2.bin" "!ide.build.path!\main2.bin"
        move /Y "!runtime.platform.path!\main3.bin" "!ide.build.path!\main3.bin"

        rem del "%param_filename%" "%counter_filename%"
		
        if exist "%timer_filename%" (
            

            for /f "usebackq delims== tokens=1,2" %%G in ("%timer_filename%") DO set %%G=%%H
            set "start_time=!start_time: =!"

            for /F "tokens=1-4 delims=:.," %%a IN ("!start_time!") DO (
                set /A "start=(((%%a*60)+%%b)*60+%%c)*100+%%d"
            )
            echo Mulai: !start_time!

            set end_time=%TIME%
            for /F "tokens=1-4 delims=:.," %%a IN ("!end_time!") DO (
                set /A "end=(((%%a*60)+%%b)*60+%%c)*100+%%d"
            )

            set /A "exec=(!end!-!start!)*10"
        
            echo Selesai: !end_time!
            echo Exec_time: !exec! miliSecond
            echo Exec_time=!exec! milisecond> "%timer_filename%"
        )

        del %counter_filename% %param_filename% %timer_filename%
         
    )
) else (
    echo Postcompile_script: ERROR rumps_runlog.txt not found
)

setlocal DISABLEDELAYEDEXPANSION