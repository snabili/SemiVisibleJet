#!/bin/bash


cat condor-jobs.jdl \
| sed -e s~MYFILENAME~X_d_800_pi_5_lt0p001.txt~ \
| sed -e s~MYPREFIX~X_d_800_pi_5_lt0p001~ \
> condor_jobs_800_5_1.jdl
condor_submit condor_jobs_800_5_1.jdl


cat condor-jobs.jdl \
| sed -e s~MYFILENAME~X_d_800_pi_5_lt0p1.txt~ \
| sed -e s~MYPREFIX~X_d_800_pi_5_lt0p1~ \
> condor_jobs_800_5_2.jdl
condor_submit condor_jobs_800_5_2.jdl



cat condor-jobs.jdl \
| sed -e s~MYFILENAME~X_d_800_pi_5_lt1.txt~ \
| sed -e s~MYPREFIX~X_d_800_pi_5_lt1~ \
> condor_jobs_800_5_3.jdl
condor_submit condor_jobs_800_5_3.jdl



cat condor-jobs.jdl \
| sed -e s~MYFILENAME~X_d_800_pi_5_lt5.txt~ \
| sed -e s~MYPREFIX~X_d_800_pi_5_lt5~ \
> condor_jobs_800_5_4.jdl
condor_submit condor_jobs_800_5_4.jdl



cat condor-jobs.jdl \
| sed -e s~MYFILENAME~X_d_800_pi_5_lt150.txt~ \
| sed -e s~MYPREFIX~X_d_800_pi_5_lt150~ \
> condor_jobs_800_5_5.jdl
condor_submit condor_jobs_800_5_5.jdl



cat condor-jobs.jdl \
| sed -e s~MYFILENAME~X_d_800_pi_5_lt300.txt~ \
| sed -e s~MYPREFIX~X_d_800_pi_5_lt300~ \
> condor_jobs_800_5_6.jdl
condor_submit condor_jobs_800_5_6.jdl



END_TIME=`/bin/date`
echo "finished at $END_TIME"
exit $exitcode
