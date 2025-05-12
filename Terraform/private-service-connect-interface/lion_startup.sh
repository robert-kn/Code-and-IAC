#! /bin/bash

sudo apt update
sudo apt install tcpdump
sudo apt install apache2 -y
sudo service apache2 restart
echo "welcome to the lion app server !!" | tee /var/www/html/index.html
EOF