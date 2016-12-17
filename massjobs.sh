#!/bin/bash


cat condor-jobs.jdl \
| sed -e s~MYFILENAME~X_d_1000_pi_2_lt0p001.txt~ \
> condor_jobs_1.jdl

condor_submit condor_jobs_1.jdl


cat condor-jobs.jdl \
| sed -e s~MYFILENAME~X_d_800_pi_2_lt0p001.txt~ \
> condor_jobs_2.jdl

condor_submit condor_jobs_2.jdl


END_TIME=`/bin/date`
echo "finished at $END_TIME"
exit $exitcode
