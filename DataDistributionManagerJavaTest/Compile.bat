@echo off

mkdir Output

"%JDK_ROOT_FOLDER%\bin\javac.exe" -Xlint:deprecation --source 8 --target 8 -cp "../Output/datadistributionmanager.jar" -d ./Output ./src/*.java
IF %ERRORLEVEL% NEQ 0 PAUSE