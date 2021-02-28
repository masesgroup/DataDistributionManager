@echo off

mkdir Output

"%JDK_ROOT_FOLDER%\bin\javac.exe" -Xlint:deprecation --source 8 --target 8 -d ./Output ./src/org/mases/datadistributionmanager/*.java ./src/org/mases/datadistributionmanager/configuration/*.java

IF %ERRORLEVEL% NEQ 0 GOTO ERROR
@echo javac END

"%JDK_ROOT_FOLDER%\bin\javac.exe" -d ./Output -Xlint:deprecation -h ..\DataDistributionManager\ ./src/org/mases/datadistributionmanager/*.java
IF %ERRORLEVEL% NEQ 0 GOTO ERROR
@echo javah END

PAUSE