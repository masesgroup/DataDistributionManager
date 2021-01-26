# Output folder

This folder contains all outputs from projects compilation:

* DataDistributionManagerNet.dll (.NET binder), datadistributionmanager.jar (Java/JVM binder)
  * x86: contains all native libraries built for 32 bit processes
  * x64: contains all native libraries built for 64 bit processes
  
The binder libraries in root folder automatically selects the bitness (x86, x64) based on the bitness of the hosted process using them.