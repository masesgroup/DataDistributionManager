@echo off

mkdir Output

"c:\Program Files\Java\%JDKVERSION%\bin\javac.exe" -Xlint:deprecation -Xlint:unchecked -d ./Output ./src/org/mases/datadistributionmanager/*.java

PAUSE