@echo off
rem this script should be post-objcopy

setlocal ENABLEDELAYEDEXPANSION

set runtime.platform.path=%~dp0
set param_filename=%~dp0rumps_compile_param.txt
set counter_filename=%~dp0rumps_runlog.txt

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
) else (
    echo Postcompile_script: ERROR rumps_compile_param.txt not found
)

if exist "%counter_filename%" (
    for /f "usebackq delims== tokens=1,2" %%G in ("%counter_filename%") DO set %%G=%%H
    set "core_count=!core_count: =!"
    echo Postcompile_script: Compiling for core-!core_count!
    
    rem save binary as main#.bin
    copy "!ide.build.path!\!source.name!.bin" "!runtime.platform.path!\main!core_count!.bin" /Y

    if !core_count! LSS 3 (
        rem less than 3, core_count++ AND call builder

        set /a core_count+=1

        echo Postcompile_script: Continue compilation for core-!core_count!
        echo core_count=!core_count!> "%counter_filename%"

        "!ide.hardware.path!\..\arduino-builder.exe" ^
            -compile ^
            -logger=machine ^
            -hardware "!ide.hardware.path!" ^
            -hardware "!rumpsduino1_0.hardware.path!" ^
            -tools "!ide.tools.builder!" ^
            -tools "!ide.tools.avr!" ^
            -built-in-libraries "!ide.libraries!" ^
            -libraries "!sketchbook.libraries.path!" ^
            -build-path "!ide.build.path!" ^
            -fqbn=!build.fqbn! ^
            -ide-version=!ide.version! ^
            -prefs=build.warn_data_percentage=!build.warn_data_percentage! ^
            -warnings=none ^
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

        del "%param_filename%" "%counter_filename%"
    )
) else (
    echo Postcompile_script: ERROR rumps_runlog.txt not found
)

setlocal DISABLEDELAYEDEXPANSION