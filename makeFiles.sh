#!/bin/bash

var=$(das_client --query="dataset=/EmergingJets_mass_X_d_2000_mass_pi_d_1_tau_pi_d_300_TuneCUETP8M1*/*/* instance=prod/phys03" --limit=0 )
echo $var
cat doit.txt \
| sed -e s~FILENAME~$var~ \
>doit.sh
chmod +x doit.sh
./doit.sh
mv lala.txt X_d_2000_pi_1_lt300.txt


var=$(das_client --query="dataset=/EmergingJets_mass_X_d_2000_mass_pi_d_1_tau_pi_d_150_TuneCUETP8M1*/*/* instance=prod/phys03" --limit=0 )
echo $var
cat doit.txt \
| sed -e s~FILENAME~$var~ \
>doit.sh
chmod +x doit.sh
./doit.sh
mv lala.txt X_d_2000_pi_1_lt150.txt


var=$(das_client --query="dataset=/EmergingJets_mass_X_d_2000_mass_pi_d_1_tau_pi_d_5_TuneCUETP8M1*/*/* instance=prod/phys03" --limit=0 )
echo $var
cat doit.txt \
| sed -e s~FILENAME~$var~ \
>doit.sh
chmod +x doit.sh
./doit.sh
mv lala.txt X_d_2000_pi_1_lt5.txt


var=$(das_client --query="dataset=/EmergingJets_mass_X_d_2000_mass_pi_d_1_tau_pi_d_1_TuneCUETP8M1*/*/* instance=prod/phys03" --limit=0 )
echo $var
cat doit.txt \
| sed -e s~FILENAME~$var~ \
>doit.sh
chmod +x doit.sh
./doit.sh
mv lala.txt X_d_2000_pi_1_lt1.txt


var=$(das_client --query="dataset=/EmergingJets_mass_X_d_2000_mass_pi_d_1_tau_pi_d_0p1_TuneCUETP8M1*/*/* instance=prod/phys03" --limit=0 )
echo $var
cat doit.txt \
| sed -e s~FILENAME~$var~ \
>doit.sh
chmod +x doit.sh
./doit.sh
mv lala.txt X_d_2000_pi_1_lt0p1.txt


var=$(das_client --query="dataset=/EmergingJets_mass_X_d_2000_mass_pi_d_1_tau_pi_d_0p001_TuneCUETP8M1*/*/* instance=prod/phys03" --limit=0 )
echo $var
cat doit.txt \
| sed -e s~FILENAME~$var~ \
>doit.sh
chmod +x doit.sh
./doit.sh
mv lala.txt X_d_2000_pi_1_lt0p001.txt



END_TIME=`/bin/date`
echo "finished at $END_TIME"
exit $exitcode
