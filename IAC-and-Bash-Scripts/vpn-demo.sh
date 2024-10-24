gcloud compute networks create network-1 --subnet-mode=custom

gcloud compute networks subnets create subnet-1a --network=network-1 --region=us-west1 --range=10.0.1.0/24

gcloud compute networks subnets create subnet-1b --network=network-1 --region=us-central1 --range=10.0.2.0/24

gcloud compute networks create network-2 --subnet-mode=custom

gcloud compute networks subnets create subnet-2a --network=network-2 --region=us-east1 --range=10.1.3.0/24

gcloud compute networks subnets create subnet-2b --network=network-2 --region=us-central1 --range=10.1.4.0/24

gcloud compute firewall-rules create allow-ssh-icmp-network-1 --direction=INGRESS --priority=1000 --network=network-1 --action=ALLOW --rules=tcp:22,icmp --source-ranges=0.0.0.0/0

gcloud compute firewall-rules create allow-ssh-icmp-network-2 --direction=INGRESS --priority=1000 --network=network-2 --action=ALLOW --rules=tcp:22,icmp --source-ranges=0.0.0.0/0

gcloud compute instances create instance-1 --zone=us-west1-b --machine-type=f1-micro --subnet=subnet-1a

gcloud compute instances create instance-2 --zone=us-central1-b --machine-type=f1-micro --subnet=subnet-1b

gcloud compute instances create instance-3 --zone=us-east1-b --machine-type=f1-micro --subnet=subnet-2a

gcloud compute instances create instance-4 --zone=us-central1-b --machine-type=f1-micro --subnet=subnet-2b

# reserve static IP's

gcloud compute addresses create network-1-static-ip --region=us-central1 --network-tier=PREMIUM

gcloud compute addresses create network-2-static-ip --region=us-central1 --network-tier=PREMIUM


echo ----------------
echo Script complete, proceed with exercise
echo ----------------
