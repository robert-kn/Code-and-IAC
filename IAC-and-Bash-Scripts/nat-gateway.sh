# See Google Cloud's documention for [setting up single NAT gateway](https://cloud.google.com/vpc/docs/special-configurations#natgateway), which is similar to the following: 


#Delete default VPC:

gcloud compute firewall-rules delete default-allow-icmp default-allow-internal default-allow-rdp default-allow-ssh -q

gcloud compute networks delete default


# Create a VPC network to host your virtual machine instances for this scenario:

gcloud compute networks create my-network --subnet-mode custom

# Create subnet for the `us-central1` region:

gcloud compute networks subnets create subnet-eu-west2 --network my-network --region europe-west2 \
        --range 192.168.1.0/24

# Create subnet for the `us-east1` region:

gcloud compute networks subnets create subnet-eu-west10 --network my-network --region europe-west10 \
        --range 192.168.2.0/24

# Create firewall rules to allow SSH connections in the new network you just created:

gcloud compute firewall-rules create my-network-allow-ssh \
    --direction=INGRESS \
    --priority=1000 \
    --network=my-network \
    --action=ALLOW \
    --rules=tcp:22 \
    --source-ranges=0.0.0.0/0

gcloud compute firewall-rules create my-network-allow-internal \
    --direction=INGRESS \
    --priority=1000 \
    --network=my-network \
    --action=ALLOW \
    --rules=all \
    --source-ranges=192.168.1.0/24,192.168.2.0/24

# Create a virtual machine to act as a NAT gateway on `my-network`:

gcloud compute instances create nat-gateway --network my-network \
    --subnet subnet-eu-west2 \
    --can-ip-forward \
    --zone europe-west2-a 

# Create a new virtual machine without an external IP address:

gcloud compute instances create private-instance \
    --network my-network \
    --subnet subnet-eu-west2 \
    --no-address \
    --zone europe-west2-b \
    --tags no-ip


#### Create a route to send traffic destined to the internet through your gateway instance:

gcloud compute routes create nat-route \
    --network my-network \
    --destination-range 0.0.0.0/0 \
    --next-hop-instance nat-gateway \
    --next-hop-instance-zone europe-west2-a \
    --tags no-ip --priority 800

# **Optional**: Log in to your NAT gateway via SSH to configure iptables to NAT traffic to the internet.

# **Note:** These examples assume the interface is called eth0. Different Linux distributions use different names for interfaces. Modify the name of the interface in commands to match your distribution.

# On your NAT gateway instance, configure iptables:

sudo sysctl -w net.ipv4.ip_forward=1
sudo iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE

# The first `sudo` command tells the kernel you want to allow IP forwarding. The second `sudo` command masquerades packets received from internal instances as if they were sent from the NAT gateway instance.

# To inspect your iptables NAT rules, use the list option:

sudo iptables -v -L -t nat

# **Optional:** If you want these settings to persist across future reboots:

sudo echo "net.ipv4.ip_forward=1" > /etc/sysctl.d/20-natgw.conf
sudo apt-get install iptables-persistent