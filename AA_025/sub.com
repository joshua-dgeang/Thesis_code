 #$ -S /bin/bash

#$ -q serial
#$ -N AA_zeroTwoFive


source /etc/profile

./a.out
