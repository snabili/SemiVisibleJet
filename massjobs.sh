#!/bin/bash

./massjobs_400_2.sh
./massjobs_600_2.sh
./massjobs_800_2.sh
./massjobs_1000_2.sh
./massjobs_1500_2.sh
./massjobs_2000_2.sh

END_TIME=`/bin/date`
echo "finished at $END_TIME"
exit $exitcode
