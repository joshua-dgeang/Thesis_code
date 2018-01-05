 #$ -S /bin/bash

#$ -q serial
#$ -N job_name

source /etc/profile

./a.out
