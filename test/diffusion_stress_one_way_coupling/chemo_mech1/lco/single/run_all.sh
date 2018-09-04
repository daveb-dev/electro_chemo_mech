#!/bin/bash
flux_rate_base='1.0e-4';
num_periods=2
time_period='0.1 0.25 0.5'
for i in $time_period
do
    dir=${i}C;
    echo $dir
    cd $dir
    mpirun.openmpi -np 4 ~/Documents/workspace_dev/electro_chemo_mech/electro_chemo_mech-opt -i lco_circle_bc.i
    cd ..
done
