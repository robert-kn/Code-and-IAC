# This script will deploy the required infra to work through the lab found 
# https://www.cloudskillsboost.google/course_templates/35/labs/432887
# Assumes you have the gcloud cli installed

# Please supply file e.g. logs when you run the script on the cmd line

MYLOG=$1

echo "Creating mynetwork VPC" > ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

gcloud compute networks create mynetwork --subnet-mode custom >> ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

echo "Creating 2 subnets in mynetwork VPC" > ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

gcloud compute networks subnets create mynetwork-euno1 --network mynetwork --region europe-north1 --range 10.128.0.0/20 >> ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

gcloud compute networks subnets create mynetwork-euwe1 --network mynetwork --region europe-west1 --range 10.132.0.0/20 >> ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

echo "Creating firewall rules for mynetwork VPC" > ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

gcloud compute firewall-rules create mynetwork-allow-icmp --network=mynetwork --direction=INGRESS --priority=1000 --action=ALLOW --rules=icmp --source-ranges=0.0.0.0/0 >> ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

gcloud compute firewall-rules create mynetwork-allow-rdp --network=mynetwork --direction=INGRESS --priority=1000 --action=ALLOW --rules=tcp:3389 --source-ranges=0.0.0.0/0 >> ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

gcloud compute firewall-rules create mynetwork-allow-ssh --network=mynetwork --direction=INGRESS --priority=1000 --action=ALLOW --rules=tcp:22 --source-ranges=0.0.0.0/0 >> ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

echo "Creating management VPC" > ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

gcloud compute networks create managementnet --subnet-mode custom >> ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

echo "Creating subnet in managementnet VPC" > ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

gcloud compute networks subnets create management-euce2 --network managementnet --region europe-central2 --range 10.130.0.0/20 >> ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

echo "Creating privatenet VPC" > ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

gcloud compute networks create privatenet --subnet-mode custom >> ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

echo "Creating subnets in privatenet VPC" > ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

gcloud compute networks subnets create privatenet-euwe2 --network privatenet --region europe-west2 --range 172.16.0.0/24 >> ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

gcloud compute networks subnets create privatenet-euwe9 --network privatenet --region europe-west9 --range 172.20.0.0/20 >> ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

echo "Creating instances in privatenet VPC" > ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

gcloud compute instances create mynet-euno1-vm --machine-type e2-micro --zone europe-north1-a --network mynetwork --subnet mynetwork-euno1 >> ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

gcloud compute instances create mynet-euwe1-vm --machine-type e2-micro --zone europe-west1-b --network mynetwork --subnet mynetwork-euwe1 >> ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

echo "Listing networks in project" >> ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

gcloud compute networks list >> ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

echo "Listing subnetworks in project" >> ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

gcloud compute networks subnets list --sort-by=NETWORK >> ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

echo "Creating firewall rules for managementnet and privatenet vpcs" >> ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

gcloud compute firewall-rules create managementnet-allow-icmp-ssh-rdp --network=managementnet --direction=INGRESS --priority=1000 --action=ALLOW --rules=icmp,tcp:3389,tcp:22 --source-ranges=0.0.0.0/0 >> ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

gcloud compute firewall-rules create privatenet-allow-icmp-ssh-rdp --network=privatenet --direction=INGRESS --priority=1000 --action=ALLOW --rules=icmp,tcp:3389,tcp:22 --source-ranges=0.0.0.0/0 >> ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

echo "Listing firewall rules in project" >> ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

gcloud compute firewall-rules list --sort-by=NETWORK >> ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

echo "Creating instances in managementnet and privatenet VPCs" >> ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

gcloud compute instances create managementnet-euce2-vm --machine-type e2-micro --zone europe-central2-a --network managementnet --subnet managementnet-euce2 >> ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

gcloud compute instances create privatenet-euwe2-vm --machine-type e2-micro --zone europe-west2-a --network privatenet --subnet privatenet-euwe2 >> ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

echo "Listing instances in a project" >> ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

echo "Retrieving the external and internal IPs of all instances in the project" >> ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

privatenet_euwe2_vm_public=$(gcloud compute instances describe privatenet-euwe2-vm --zone europe-west2-a | grep natIP | awk '{print $2}')
privatenet_euwe2_vm_internal=$(gcloud compute instances describe privatenet-euwe2-vm --zone europe-west2-a | grep networkIP | awk '{print $2}')

mynet_euwe1_vm_public=$(gcloud compute instances describe mynet-euwe1-vm --zone europe-west1-b | grep natIP | awk '{print $2}')
mynet_euwe1_vm_internal=$(gcloud compute instances describe mynet-euwe1-vm --zone europe-west1-b | grep networkIP | awk '{print $2}')

mynet_euno1_vm_public=$(gcloud compute instances describe mynet-euno1-vm --zone europe-north1-a | grep natIP | awk '{print $2}')
mynet_euno1_vm_internal=$(gcloud compute instances describe mynet-euno1-vm --zone europe-north1-a | grep networkIP | awk '{print $2}')

managementnet_euce2_vm_public=$(gcloud compute instances describe managementnet-euce2-vm --zone europe-central2-a | grep natIP | awk '{print $2}')
managementnet_euce2_vm_internal=$(gcloud compute instances describe managementnet-euce2-vm --zone europe-central2-a | grep networkIP | awk '{print $2}')

echo "Exploring connectivity between my local and instances public IPs in the various VPCs within the project" >> ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

ping -c 3 $privatenet_euwe2_vm_public >> ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

ping -c 3 $mynet_euwe1_vm_public >> ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

ping -c 3 $mynet_euno1_vm_public >> ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

ping -c 3 $managementnet_euce2_vm_public >> ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

echo "Exploring connectivity between my local and instances internal IPs in the various VPCs within the project" >> ./$MYLOG.txt

ping -c 3 $privatenet_euwe2_vm_internal >> ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

ping -c 3 $mynet_euwe1_vm_internal >> ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

ping -c 3 $mynet_euno1_vm_internal >> ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

ping -c 3 $managementnet_euce2_vm_internal >> ./$MYLOG.txt
