#!/bin/bash

./massjobs_400_1.sh
./massjobs_600_1.sh
./massjobs_800_1.sh
./massjobs_1000_1.sh
./massjobs_1500_1.sh
./massjobs_2000_1.sh


./massjobs_400_2.sh
./massjobs_600_2.sh
./massjobs_800_2.sh
./massjobs_1000_2.sh
./massjobs_1500_2.sh
./massjobs_2000_2.sh


./massjobs_400_5.sh
./massjobs_600_5.sh
./massjobs_800_5.sh
./massjobs_1000_5.sh
./massjobs_1500_5.sh
./massjobs_2000_5.sh


./massjobs_400_10.sh
./massjobs_600_10.sh
./massjobs_800_10.sh
./massjobs_1000_10.sh
./massjobs_1500_10.sh
./massjobs_2000_10.sh


END_TIME=`/bin/date`
echo "finished at $END_TIME"
exit $exitcode
