#!/bin/bash



cat condor-jobs.jdl \
| sed -e s~MYFILENAME~X_d_1500_pi_2_lt0p001.txt~ \
| sed -e s~MYPREFIX~X_d_1500_pi_2_lt0p001~ \
> condor_jobs_1.jdl
condor_submit condor_jobs_2.jdl



END_TIME=`/bin/date`
echo "finished at $END_TIME"
exit $exitcode
