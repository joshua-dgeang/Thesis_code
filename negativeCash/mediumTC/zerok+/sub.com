#$ -S /bin/bash

#$ -q serial
#$ -N ZerokP   

source /etc/profile

./a.out
