@echo off

mkdir Output

"c:\Program Files\Java\%JDKVERSION%\bin\javac.exe" -cp "../Output/JCOBridge.jar" -d ./Output ./src/*.java ./src/org/mases/jcobridge/tests/*.java
IF %ERRORLEVEL% NEQ 0 PAUSE