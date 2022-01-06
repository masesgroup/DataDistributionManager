# Development Status

* The project supports, at this stage of implementation, these communication subsystems:
  * Apache Kafka througth the library available from https://www.nuget.org/packages/librdkafka.redist/
  * OpenDDS compiled from the source available in https://github.com/objectcomputing/OpenDDS/

* The project can be used from the most used programming languages:
  * Bindings are available for C/C++, Java and .NET

* There is a basic implementation of mastership management for leadership election.

* The CI/CD uses:
  * VS2019 image
  * C/C++ projects are built with Visual Studio 2019
  * NuGet packages hosts version for .NET Framework, .NET Core 3.1, .NET 5 and latest .NET 6
  * Maven produces and deploy artifacts compiled for Java 11 and above.
