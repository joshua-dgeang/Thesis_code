 #$ -S /bin/bash

#$ -q serial
#$ -N highTC

source /etc/profile

./a.out
