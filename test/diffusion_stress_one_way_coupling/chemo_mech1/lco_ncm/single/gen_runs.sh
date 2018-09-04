#!/bin/bash
flux_rate_base='1.0e-4';
num_periods=1
time_period='1'
pressure='0.0 10e-6 50e-6 100e-6 200e-6'
for i in $time_period
do
    dir=${i}C;
    echo $dir
    period=$(python -c 'import sys; print 2.0*3600.0/float(sys.argv[1])' "$i")
    flux_rate=$(python -c 'import sys; print (float(sys.argv[2])*float(sys.argv[1]))' "$i" "$flux_rate_base")
    total_time=$(python -c 'import sys; print float(sys.argv[2])*2.0*3600.0/float(sys.argv[1])' "$i" "$num_periods")
    echo $period $flux_rate $total_time
    for j in $pressure
    do
      pr=pres_$j
      if [ -d $dir/$pr ]
      then
        echo "pressure directory exists"
      else
        mkdir $dir/$pr
      fi
      pwd
      sed "s/flux_rate/$flux_rate/g" lco_circle_bc.i | sed "s/t_period/$period/g" | sed "s/total_time/$total_time/g" | sed "s/pressure_value/$j/g" > $dir/$pr/lco_circle_bc.i
      cp *msh $dir/$pr
    done
done
