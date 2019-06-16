sudo ifconfig usb0 192.168.7.2
sudo route add default gw 192.168.7.1
echo "nameserver 8.8.8.8" | sudo tee /etc/resolv.conf > /dev/null





