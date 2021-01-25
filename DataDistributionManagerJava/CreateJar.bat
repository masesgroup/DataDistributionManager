@echo off

rmdir /Q /S Output
mkdir Output

rmdir /Q /S Javadoc
mkdir Javadoc

"c:\Program Files\Java\%JDKVERSION%\bin\javac.exe" -Werror -d ./Output ./src/org/mases/datadistributionmanager/*.java

IF %ERRORLEVEL% NEQ 0 GOTO ERROR
@echo javac END
"c:\Program Files\Java\%JDKVERSION%\bin\javadoc.exe" -quiet -author -public -d ./Javadoc -link https://docs.oracle.com/javase/8/docs/api/ ./src/org/mases/datadistributionmanager/*.java
IF %ERRORLEVEL% NEQ 0 GOTO ERROR
@echo javadoc END
"c:\Program Files\Java\%JDKVERSION%\bin\javah.exe" -cp ./Output -v -d ..\DataDistributionManager\ org.mases.datadistributionmanager.NativeInterface
IF %ERRORLEVEL% NEQ 0 GOTO ERROR
@echo javah END
"c:\Program Files\Java\%JDKVERSION%\bin\jar.exe" cvfm datadistributionmanager.jar datadistributionmanagerManifest.txt -C ./Output . > .\Output\datadistributionmanager.jar.txt
IF %ERRORLEVEL% NEQ 0 GOTO ERROR
@echo jar datadistributionmanager.jar END
"c:\Program Files\Java\%JDKVERSION%\bin\jar.exe" cvf datadistributionmanager.docs.jar -C ./Javadoc . > .\Output\datadistributionmanager.docs.jar.txt 
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