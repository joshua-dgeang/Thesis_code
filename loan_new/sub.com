 #$ -S /bin/bash

#$ -q serial
#$ -N zeroSeven


source /etc/profile

./a.out
