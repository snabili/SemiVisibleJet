#!/bin/bash


cat condor-jobs.jdl \
| sed -e s~MYFILENAME~X_d_600_pi_2_lt0p001.txt~ \
| sed -e s~MYPREFIX~X_d_600_pi_2_lt0p001~ \
> condor_jobs_600_2_1.jdl
condor_submit condor_jobs_600_2_1.jdl


cat condor-jobs.jdl \
| sed -e s~MYFILENAME~X_d_600_pi_2_lt0p1.txt~ \
| sed -e s~MYPREFIX~X_d_600_pi_2_lt0p1~ \
> condor_jobs_600_2_2.jdl
condor_submit condor_jobs_600_2_2.jdl



cat condor-jobs.jdl \
| sed -e s~MYFILENAME~X_d_600_pi_2_lt1.txt~ \
| sed -e s~MYPREFIX~X_d_600_pi_2_lt1~ \
> condor_jobs_600_2_3.jdl
condor_submit condor_jobs_600_2_3.jdl



cat condor-jobs.jdl \
| sed -e s~MYFILENAME~X_d_600_pi_2_lt5.txt~ \
| sed -e s~MYPREFIX~X_d_600_pi_2_lt5~ \
> condor_jobs_600_2_4.jdl
condor_submit condor_jobs_600_2_4.jdl



cat condor-jobs.jdl \
| sed -e s~MYFILENAME~X_d_600_pi_2_lt150.txt~ \
| sed -e s~MYPREFIX~X_d_600_pi_2_lt150~ \
> condor_jobs_600_2_5.jdl
condor_submit condor_jobs_600_2_5.jdl



cat condor-jobs.jdl \
| sed -e s~MYFILENAME~X_d_600_pi_2_lt300.txt~ \
| sed -e s~MYPREFIX~X_d_600_pi_2_lt300~ \
> condor_jobs_600_2_6.jdl
condor_submit condor_jobs_600_2_6.jdl



END_TIME=`/bin/date`
echo "finished at $END_TIME"
exit $exitcode
