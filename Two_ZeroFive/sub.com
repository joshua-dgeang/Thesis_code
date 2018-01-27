 #$ -S /bin/bash

#$ -q serial
#$ -N Two_Five


source /etc/profile

./a.out
