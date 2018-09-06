#!/bin/bash
ip_range="33.0.0.0"
ifconfig lo:0 $ip_range
export PATH=/usr/local/lib:$PATH
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
#make -C ./code/ > /dev/null
echo 500000 > /proc/sys/kernel/threads-max
echo 800000 > /proc/sys/vm/max_map_count
echo 1000000 > /proc/sys/kernel/pid_max 
echo 2000 65000 > /proc/sys/net/ipv4/ip_local_port_range
#echo 1 > /proc/sys/net/ipv4/tcp_abort_on_overflow
echo 10 > /proc/sys/net/ipv4/tcp_fin_timeout
echo 5000 > /proc/sys/net/ipv4/tcp_max_syn_backlog
echo 1 > /proc/sys/net/ipv4/tcp_tw_reuse
echo 1 > /proc/sys/net/ipv4/tcp_tw_recycle
#echo 01 >  /proc/irq/18/smp_affinity
echo 0 > /proc/sys/kernel/hung_task_timeout_secs
echo 4096 > /proc/sys/net/core/somaxconn  
echo fq > /proc/sys/net/core/default_qdisc
echo 11 > /sys/class/net/eth0/queues/rx-0/rps_cpus
#echo 300 > /proc/sys/net/ipv4/tcp_keepalive_time
#echo 10 > /proc/sys/net/ipv4/tcp_keepalive_intvl
#echo 20 > /proc/sys/net/ipv4/tcp_keepalive_probes
#echo 1 > /proc/sys/net/ipv4/tcp_ecn
#echo 1 > /proc/sys/net/ipv4/tcp_dsack
#echo 1 > /proc/sys/net/ipv4/tcp_fack
#echo 1 > /proc/sys/net/ipv4/tcp_rfc1337
#echo 0 > /proc/sys/net/ipv4/tcp_syncookies
ifconfig eth0 txqueuelen 4096
ethtool -G eth0 rx 4096
ethtool -G eth0 tx 4096
ethtool -C eth0 rx-usecs 30 
#ifconfig eth2 mtu 9000
ulimit -s 40960
ulimit -u 1000000
ulimit -n 1000000
ulimit -a
#code to simulate latency
tc qdisc add dev eth0 root netem delay 20ms 2ms
#sudo tc qdisc add dev lo:0 root netem delay 50ms 25ms
tc qdisc change dev eth0 root netem delay 20ms 2ms
#sudo tc qdisc change dev lo:0 root netem delay 20ms 10ms
#service irqbalance stop
echo -e "\nFollowing IP Range is enabled:$ip_range"
./wolfsslserver_multithreaded/code/wolfsslserver -p 9999 -d ./wolfsslserver_multithreaded/cert/ -i 553844736 -s 17000 >/dev/null 2>&1 &

