 #$ -S /bin/bash

#$ -q serial
#$ -N interest0045 


source /etc/profile

./a.out
