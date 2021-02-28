# Welcome to **Data Distribution Manager**

A reliable subsystem able to distribute data across multiple datacenters using multiple technologies.
It is the core of [Sinapse](https://www.sinapsesystem.com) High Avalailablity, now we made it available for everyone.

This project adheres to the Contributor [Covenant code of conduct](https://github.com/masesgroup/DataDistributionManager/blob/master/CODE_OF_CONDUCT.md). By participating, you are expected to uphold this code. Please report unacceptable behavior to coc_reporting@masesgroup.com.

Actual implementation can use [Apache Kafka](https://kafka.apache.org/) or [OpenDDS](https://opendds.org/) as back-end.

## Scope of the project

This project aims to create a library set, exposing a simple interface, to manage data distribution across multiple processes over multiple datacenters.

## Actual state

* It can be used from C/C++, Java, .NET
* There is a basic implementation of mastership management for leadership election.