# Data Distribution Manager

A reliable subsystem able to distribute data across multiple datacenters using multiple technologies.
It is the core of [Sinapse](https://www.sinapsesystem.com) High Avalailablity, now we made it available for everyone.

This project adheres to the Contributor [Covenant code of conduct](CODE_OF_CONDUCT.md). By participating, you are expected to uphold this code. Please report unacceptable behavior to coc_reporting@masesgroup.com.

Actual implementation can use [Apache Kafka](https://kafka.apache.org/) or [OpenDDS](https://opendds.org/) as back-end.

## Scope of the project

This project aims to create a library set, exposing a simple interface, to manage data distribution across multiple processes over multiple datacenters.

## Actual state

* It can be used from C/C++, Java, .NET
* There is a basic implementation of mastership management for leadership election.

## Build sequence

**Until now the generation can be done manually. Soon will be setted-up an automatic workflow.**

* Clone the repository

* Open _VisualStudio Developer Command Prompt_

* Retrieves the OpenDDS implementation from github repository:

```
git.exe submodule update --init -- "third_party/OpenDDS"
```

* Build the OpenDDS libraries:

```
cd third_party\OpenDDS

configure

msbuild -p:Configuration=Release,Platform=x64 -m DDS_TAOv2.sln

xcopy .\lib\*.dll ..\..\Output\x64\
xcopy .\lib\*.pdb ..\..\Output\pdb64\
xcopy .\lib\*.lib ..\..\Output\lib64\
xcopy .\bin\*.exe ..\..\Output\bin64\
xcopy .\bin\*.pdb ..\..\Output\pdb64\
xcopy .\ACE_wrappers\lib\*.dll ..\..\Output\x64\
xcopy .\ACE_wrappers\lib\*.pdb ..\..\Output\pdb64\
xcopy .\ACE_wrappers\lib\*.lib ..\..\Output\lib64\
xcopy .\ACE_wrappers\bin\*.exe ..\..\Output\bin64\
xcopy .\ACE_wrappers\bin\*.pdb ..\..\Output\pdb64\

msbuild -p:Configuration=Release,Platform=Win32 -m DDS_TAOv2.sln

xcopy .\lib\*.dll ..\..\Output\x86\
xcopy .\lib\*.pdb ..\..\Output\pdb32\
xcopy .\lib\*.lib ..\..\Output\lib32\
xcopy .\bin\*.exe ..\..\Output\bin32\
xcopy .\bin\*.pdb ..\..\Output\pdb32\
xcopy .\ACE_wrappers\lib\*.dll ..\..\Output\x86\
xcopy .\ACE_wrappers\lib\*.pdb ..\..\Output\pdb32\
xcopy .\ACE_wrappers\lib\*.lib ..\..\Output\lib32\
xcopy .\ACE_wrappers\bin\*.exe ..\..\Output\bin32\
xcopy .\ACE_wrappers\bin\*.pdb ..\..\Output\pdb32\

cd ..\..
```

* Generate the projects related to OpenDDS transport:

```
cd src\DataDistributionManagerOpenDDS
GenerateFile.cmd
cd ..\..
```

* Restore Nuget packages for solution DataDistributionManager.sln (or use nuget.exe or Visual Studio) to compile Apache Kafka transport
Restore NuGet Packages from command line (not needed if Visual Studio is used):

> NOTE: NuGet.exe can be download from [NuGet](https://www.nuget.org/downloads)
```
nuget.exe restore src\DataDistributionManager.sln
```

* Builds the Data Distribution Manager:

  * If the build machine hosts a JDK it is possible to use JNI headers from that JDK with the following command:
  ```  
  set JDK_ROOT_FOLDER=C:\Program Files\Java\jdk-13.0.2+8
  ```

```
msbuild /m -p:Configuration=Release,Platform=x64 src\DataDistributionManager.sln
msbuild /m -p:Configuration=Release,Platform=Win32 src\DataDistributionManager.sln
```
* To generate Java binder execute:

> NOTE: a JDK must be installed in the build machine:

```
cd src\DataDistributionManagerJava
set JDK_ROOT_FOLDER=C:\Program Files\Java\jdk-13.0.2+8
CreateJars.bat
cd ..\..
```