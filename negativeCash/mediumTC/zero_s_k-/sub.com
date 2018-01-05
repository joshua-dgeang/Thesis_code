#$ -S /bin/bash

#$ -q serial
#$ -N ZerokN   

source /etc/profile

./a.out
