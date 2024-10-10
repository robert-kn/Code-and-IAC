gcloud compute instances delete privatenet-euwe2-vm --zone europe-west2-a --quiet
gcloud compute instances delete managementnet-euce2-vm --zone europe-central2-a -q
gcloud compute instances delete mynet-euwe1-vm --zone europe-west1-b -q
gcloud compute instances delete mynet-euno1-vm --zone europe-north1-a -q
gcloud compute firewall-rules delete mynetwork-allow-icmp -q
gcloud compute firewall-rules delete mynetwork-allow-rdp -q
gcloud compute firewall-rules delete mynetwork-allow-ssh -q
gcloud compute firewall-rules delete managementnet-allow-icmp-ssh-rdp -q
gcloud compute firewall-rules delete privatenet-allow-icmp-ssh-rdp -q
gcloud compute networks subnets delete mynetwork-euwe1 --region europe-west1 -q
gcloud compute networks subnets delete mynetwork-euno1 --region europe-north1 -q
gcloud compute networks subnets delete management-euce2 --region europe-central2 -q
gcloud compute networks subnets delete privatesubnet-euwe2 --region europe-west2 -q
gcloud compute networks subnets delete privatesubnet-euwe9 --region europe-west9 -q
gcloud compute networks delete privatenet -q
gcloud compute networks delete managementnet -q
gcloud compute networks delete mynetwork -q