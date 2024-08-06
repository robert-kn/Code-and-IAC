gcloud compute instances delete blue-euno1-vm --zone europe-north1-a --quiet
gcloud compute instances delete test-vm --zone europe-central2-a -q
gcloud compute instances delete green-euwe1-vm --zone europe-west1-b -q
gcloud compute firewall-rules delete allow-http-web-server -q
gcloud compute networks subnets delete mynetwork-euce2 --region europe-central2 -q
gcloud compute networks subnets delete mynetwork-euwe1 --region europe-west1 -q
gcloud compute networks subnets delete mynetwork-euno1 --region europe-north1 -q
gcloud compute networks delete mynetwork -q
PROJECT_ID=$(gcloud config get-value project)
gcloud iam service-accounts delete network-admin@$PROJECT_ID.iam.gserviceaccount.com -q