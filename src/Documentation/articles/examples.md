# DataDistributionManager example 

>
> NOTE: the examples use OpenDDS to avoid an external Apache Kafka server
>

## .NET

To execute the *dotnet* example do the following steps:
1. open a windows command prompt
2. cd to the examples/dotnet/TransmitterReceiver folder

```
$> cd [PATH TO EXAMPLES FOLDER]
$> cd dotnet\TransmitterReceiver
```

3. build the .NET solution
```
$> dotnet build TransmitterReceiver.csproj
```

This command will restore the **NuGet DataDistributionManager package** and build project for all the frameworks declared in the project, a **TransmitterReceiver.exe** file will be placed in the framework specific output folder. (e.g. .\dotnet\TransmitterReceiver\bin\Debug\net5.0\)

4. cd to the output folder
```
$> cd bin\Debug\net5.0\
```
5. open another windows command prompt and **cd to the output folder**
6. in the first command prompt execute the example as listener
```
$> TransmitterReceiver.exe
```
This will also start the DCPSInfoRepo.exe

6. in the second command prompt execute the example as transmitter
```
$> TransmitterReceiver.exe t
```
This start the application in transmission mode, in the other command prompt is visible the start of communications.

## Java

To execute the *java* example do the following steps:
1. open a windows command prompt
2. cd to the examples/java/TransmitterReceiver folder

```
$> cd [PATH TO EXAMPLES FOLDER]
$> cd java\TransmitterReceiver
```
3. If needed set the %JDK_ROOT_FOLDER% environment variable to your jdk folder (e.g. C:\Program Files\Java\jdk-15.0.2)
```
$> set JDK_ROOT_FOLDER=[YOUR_PATH_TO_JDK_FOLDER]
```
4. Put the **datadistributionmanager.jar** file in the application folder, the file is available at [latest release](https://github.com/masesgroup/DataDistributionManager/releases/latest)
5. Compile the java source
```
$> compile.bat
```
6. execute the example as receiver
```
$> startreceiver.bat
```
This start the receiver and will also start the **DCPSInfoRepo.exe**

7. open a new windows command prompt and do steps 2 and 3
8. execute the example as transmitter
```
$> starttransmitter.bat
```
This start the application in transmission mode in the new terminal window, in the other terminal window is visible the start of communications.

