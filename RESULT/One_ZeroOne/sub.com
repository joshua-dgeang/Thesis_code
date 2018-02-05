 #$ -S /bin/bash

#$ -q serial
#$ -N One_One 

source /etc/profile

./a.out
