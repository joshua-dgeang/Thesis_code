#$ -S /bin/bash

#$ -q serial
#$ -N ZerokPandkN   

source /etc/profile

./a.out
