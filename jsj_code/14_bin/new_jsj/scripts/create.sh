mdadm /dev/md0 -C -l0 -n3 --assume-clean /dev/nvme[0-2]n1

echo y | mkfs -t ext4 /dev/md0

mount /dev/md0 /mnt/pool1/fs1

