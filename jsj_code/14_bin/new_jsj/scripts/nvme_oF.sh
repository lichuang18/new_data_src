#!/bin/bash

# 设置SSH登录的目标主机、用户名和密码
data_src_dev="10.0.50.22"

data_src_user="root"

data_src_password="1"

storage_dev="10.0.50.14"

storage_user="root"

storage_password="@cetc52."

client_dev="10.0.50.4"

client_user="root"

client_password="1"


echo "Remove RAID Config"
./creat_raid.sh remove

echo "Server Exec"

sshpass -p "$storage_password" ssh "$storage_user@$storage_dev"  '/root/bin/scripts/server.sh'

sleep 5
echo "Client Exec"

sshpass -p "$client_password" ssh "$client_user@$client_dev"  './client.sh'
