 #$ -S /bin/bash

#$ -q serial
#$ -N One_Five

source /etc/profile

./a.out
