# Lab learning objectives:

# 1. Create Custom Network and Subnets in Cloud Shell
# 2. Define Firewall Rule to Allow SSH
# 3. Spin Up VM Instances
# 4. Test via SSH

gcloud compute networks create ps-network --subnet-mode custom

gcloud compute networks subnets create ps-subnet-us-east --network ps-network --region us-east1 --range 10.0.1.0/24

gcloud compute networks subnets create ps-subnet-eu-west --network ps-network --region europe-west1 --range 10.0.2.0/24

gcloud compute networks subnets list --network ps-network

gcloud compute firewall-rules create ps-allow-ssh --network ps-network --allow tcp:22,icmp --source-ranges 0.0.0.0/0

gcloud compute instances create ps-vm-us --subnet ps-subnet-us-east --zone us-east1-b --machine-type e2-micro

gcloud compute instances create ps-vm-eu --subnet ps-subnet-eu-west --zone europe-west1-b --machine-type e2-micro

export EXTERNAL_IP=$(gcloud compute instances describe ps-vm-eu --zone europe-west1-b | grep natIP | awk '{print $2}')

export INTERNAL_IP=$(gcloud compute instances describe ps-vm-eu --zone europe-west1-b | grep networkIP | awk '{print $2}')

gcloud compute ssh ps-vm-us --zone us-east1-b

ping -c 3 $EXTERNAL_IP

ping -c 3 $INTERNAL_IP