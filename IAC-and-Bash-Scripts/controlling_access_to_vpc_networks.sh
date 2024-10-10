MYLOG=$1

echo "Creating mynetwork VPC" > ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

gcloud compute networks create mynetwork --subnet-mode custom >> ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

echo "Creating 3 subnets in mynetwork VPC" > ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

gcloud compute networks subnets create mynetwork-euno1 --network mynetwork --region europe-north1 --range 10.128.0.0/20 >> ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

gcloud compute networks subnets create mynetwork-euwe1 --network mynetwork --region europe-west1 --range 10.132.0.0/20 >> ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

gcloud compute networks subnets create mynetwork-euce2 --network mynetwork --region europe-centra2 --range 10.130.0.0/20 >> ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

gcloud compute instances create blue-euno1-vm --machine-type e2-micro --zone europe-north1-a --network mynetwork --subnet mynetwork-euno1 --tags web-server >> ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

gcloud compute instances create green-euwe1-vm --machine-type e2-micro --zone europe-west1-b --network mynetwork --subnet mynetwork-euwe1  >> ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

gcloud compute firewall-rules create allow-http-web-server --network=mynetwork --direction=INGRESS --priority=1000 --action=ALLOW --rules=tcp:80 --source-ranges=0.0.0.0/0 --source-tags web-server >> ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

gcloud compute instances create test-vm --machine-type e2-micro --zone europe-central2-a --network mynetwork --subnet mynetwork-euce2  >> ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

gcloud iam service-accounts create network-admin --display-name network-admin-sa >> ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

PROJECT_ID=$(gcloud config get-value project)

gcloud projects add-iam-policy-binding $PROJECT_ID --member serviceAccount:network-admin@$PROJECT_ID.iam.gserviceaccount.com --role roles/compute.networkAdmin >> ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

gcloud projects remove-iam-policy-binding $PROJECT_ID --member serviceAccount:network-admin@$PROJECT_ID.iam.gserviceaccount.com --role roles/compute.networkAdmin >> ./$MYLOG.txt

echo "\n" >> ./$MYLOG.txt

gcloud projects add-iam-policy-binding $PROJECT_ID --member serviceAccount:network-admin@$PROJECT_ID.iam.gserviceaccount.com --role roles/compute.securityAdmin >> ./$MYLOG.txt
