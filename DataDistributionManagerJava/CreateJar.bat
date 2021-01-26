@echo off

rmdir /Q /S Output
mkdir Output

rmdir /Q /S Javadoc
mkdir Javadoc

"%JDK_ROOT_FOLDER%\bin\javac.exe" -Xlint:deprecation --source 8 --target 8 -d ./Output ./src/org/mases/datadistributionmanager/*.java
IF %ERRORLEVEL% NEQ 0 GOTO ERROR
@echo javac END
"%JDK_ROOT_FOLDER%\bin\javadoc.exe" --source 8 -quiet -author -public -d ./Javadoc -link https://docs.oracle.com/javase/8/docs/api/ ./src/org/mases/datadistributionmanager/*.java
IF %ERRORLEVEL% NEQ 0 GOTO ERROR
@echo javadoc END
"%JDK_ROOT_FOLDER%\bin\jar.exe" cvfm datadistributionmanager.jar datadistributionmanagerManifest.txt -C ./Output . > .\Output\datadistributionmanager.jar.txt
IF %ERRORLEVEL% NEQ 0 GOTO ERROR
@echo jar datadistributionmanager.jar END
"%JDK_ROOT_FOLDER%\bin\jar.exe" cvf datadistributionmanager.docs.jar -C ./Javadoc . > .\Output\datadistributionmanager.docs.jar.txt 
IF %ERRORLEVEL% NEQ 0 GOTO ERROR
@echo jar datadistributionmanager.docs.jar END
move /Y datadistributionmanager.jar ..\Output
IF %ERRORLEVEL% NEQ 0 GOTO ERROR
@echo move datadistributionmanager.jar END
move /Y datadistributionmanager.docs.jar ..\Output
IF %ERRORLEVEL% NEQ 0 GOTO ERROR
@echo move datadistributionmanager.docs.jar END
GOTO END

:ERROR
@echo failed
PAUSE
exit /b 1
:END
exit /b 0