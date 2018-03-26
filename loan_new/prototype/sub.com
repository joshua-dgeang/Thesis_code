 #$ -S /bin/bash

#$ -q serial
#$ -N interest003 


source /etc/profile

./a.out
