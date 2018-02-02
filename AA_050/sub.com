 #$ -S /bin/bash

#$ -q serial
#$ -N AA_zeroFive


source /etc/profile

./a.out
