 #$ -S /bin/bash

#$ -q serial
#$ -N lowTC

source /etc/profile

./a.out
