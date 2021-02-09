@echo off
perl %ACE_ROOT%\bin\generate_export_file.pl DataDistributionSchemaCommon > DataDistributionSchemaCommon_Export.h
perl %ACE_ROOT%/bin/mwc.pl -type vc14 DataDistributionManagerOpenDDS.mwc
