cd /opt/spdk

./build/bin/nvmf_tgt &

./scripts/rpc.py bdev_aio_create /dev/nvme0n1 NVME1
./scripts/rpc.py nvmf_create_transport -t TCP -u 16384 -m 8 -c 8192

./scripts/rpc.py nvmf_create_subsystem nqn.2023-06.io.spdk:cnode1 -a -s SPDK00000000000001 -d SPDK_Controller1

./scripts/rpc.py nvmf_subsystem_add_ns nqn.2023-06.io.spdk:cnode1 NVME1

./scripts/rpc.py nvmf_subsystem_add_listener nqn.2023-06.io.spdk:cnode1 -t TCP -a 10.0.50.14 -s 4420
