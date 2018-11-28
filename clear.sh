for i in `ipcs -s | awk '{ print $2 }'`;
do
ipcrm -s $i;
done

for i in `ipcs -m | awk '{ print $2 }'`;
do
ipcrm -m $i;
done

for i in `ipcs -q | awk '{ print $2 }'`;
do
ipcrm -q $i;
done
