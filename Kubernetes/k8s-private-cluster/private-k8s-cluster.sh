### Create custom VPC network with 2 subnets and secondary IP ranges:

    gcloud compute networks create custom-network --subnet-mode=custom

    gcloud compute networks subnets create subnet-a --network=custom-network --region=us-central1 --range=192.168.25.0/24 --secondary-range=pod-range=10.2.0.0/16,service-range=10.3.0.0/16 --enable-private-ip-google-access

    gcloud compute networks subnets create subnet-b --network=custom-network --region=europe-west2 --range=10.255.2.0/24

# should I enable private google access in subnet b for communication with the k8s cluster once I add the subnet
# as a master authorised network? try it with it enabled and without it enabled

### Create a private k8s cluster 

gcloud services enable container.googleapis.com

gcloud container clusters create private-cluster \
        --enable-master-authorized-networks \
        --enable-ip-alias \
        --enable-private-nodes \
        --master-ipv4-cidr 172.16.0.0/28 \
        --subnetwork subnet-a \
        --cluster-secondary-range-name "pod-range" \
        --services-secondary-range-name "service-range" \
        --zone us-central1-a  \
        --no-enable-basic-auth \
        --no-issue-client-certificate

### Create 2 compute engine instances; one in each subnet

gcloud compute instances create different-subnet-vm \
                    --machine-type e2-micro --zone europe-west2-b --network custom-network \
                    --subnet subnet-b

gcloud compute instances create same-subnet-vm \
                    --machine-type e2-micro --zone us-central1-a --network custom-network \
                    --subnet subnet-a

### ssh into the same-subnet-vm and run the command below to get the cluster credentials

gcloud container clusters get-credentials private-cluster --internal-ip --zone us-central1-a

### then run a kubectl command such as the one below once you are logged in

# kubectl get no

### exit out of the same-subnet-vm and ssh into the different-subnet-vm and run the same commands as above. 
### expect them to fail because the internal ip range for subnet b has not been added as master authorised network
### for the cluster

### exit out of the different-subnet-vm and start the cloud shell instance and retrieve its externaal ip address
### and save it as follows export EXTERNAL_IP=$(curl icanhazip.com) or 
### export EXTERNAL_IP=$(dig +short myip.opendns.com @resolver1.opendns.com)

### run the command below in cloud shell

### gcloud container clusters get-credentials private-cluster --zone us-central1-a

### then run kubectl get no in cloud shell

### it should fail

### add the EXTERNAL_IP of cloud shell and the ip range of subnet b using the following commands to the cluster

# gcloud container clusters update private-cluster --zone [CLUSTER-ZONE]\
#        --enable-master-authorized-networks \
#        --master-authorized-networks (first-ip-range),(second-ip-address))


### Repeat the process of trying to access the cluster in the different-subnet-vm and cloud shell again.
### both should be successful this time!

### spin down your resources