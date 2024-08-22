modprobe nvme-fabrics
modprobe nvme-tcp

nvme discover -t tcp -a 10.0.50.14 -s 4420

nvme connect -t tcp -n "nqn.2023-06.io.spdk:cnode1" -a 10.0.50.14 -s 4420

#nvme disconnect -n "nqn.2023-06.io.spdk:cnode1"
