# Create firewall rules allowing http and load balancer/health check access

gcloud compute firewall-rules create http-allow --direction=INGRESS --priority=1000 --network=default --action=ALLOW --rules=tcp:80 --source-ranges=0.0.0.0/0 --target-tags=http-server

gcloud compute firewall-rules create health-check-allow --direction=INGRESS --priority=1000 --network=default --action=ALLOW --rules=tcp --source-ranges=130.211.0.0/22,35.191.0.0/16 --target-tags=http-server

# create version 1 template

gcloud compute instance-templates create template-v1 --machine-type=e2-micro --metadata=startup-script-url=gs://acg-gcloud-course-resources/network-engineer/mig-startup-v1.sh --tags=http-server --boot-disk-device-name=template-v1

# create health check and groups from version 1 template

gcloud compute health-checks create tcp "health-check" --timeout "5" --check-interval "10" --unhealthy-threshold "3" --healthy-threshold "2" --port "80"

gcloud beta compute instance-groups managed create us-central1-group --base-instance-name=us-central1-group --template=template-v1 --size=1 --zones=us-central1-b,us-central1-c,us-central1-f --instance-redistribution-type=PROACTIVE --health-check=health-check --initial-delay=300

gcloud beta compute instance-groups managed set-autoscaling "us-central1-group" --region "us-central1" --cool-down-period "60" --max-num-replicas "4" --min-num-replicas "2" --target-cpu-utilization "0.6"

gcloud beta compute instance-groups managed create europe-west1-group --base-instance-name=europe-west1-group --template=template-v1 --size=1 --zones=europe-west1-b,europe-west1-c,europe-west1-d --instance-redistribution-type=PROACTIVE --health-check=health-check --initial-delay=300

gcloud beta compute instance-groups managed set-autoscaling "europe-west1-group" --region "europe-west1" --cool-down-period "60" --max-num-replicas "4" --min-num-replicas "2" --target-cpu-utilization "0.6"


# set names ports for instance groups

gcloud compute instance-groups managed set-named-ports europe-west1-group \
    --named-ports http:80 \
    --region europe-west1

gcloud compute instance-groups managed set-named-ports us-central1-group \
    --named-ports http:80 \
    --region us-central1

# create backend service and add backends

gcloud compute backend-services create http-backend \
    --protocol HTTP \
    --health-checks health-check \
    --enable-logging \
    --logging-sample-rate=1.0 \
    --global

gcloud compute backend-services add-backend http-backend \
    --balancing-mode=RATE \
    --max-rate-per-instance=50 \
    --capacity-scaler=1 \
    --instance-group=europe-west1-group \
    --instance-group-region=europe-west1 \
    --global

gcloud compute backend-services add-backend http-backend \
    --balancing-mode=RATE \
    --max-rate-per-instance=50 \
    --capacity-scaler=1 \
    --instance-group=us-central1-group \
    --instance-group-region=us-central1 \
    --global

#create lb

gcloud compute url-maps create http-lb \
    --default-service http-backend

gcloud compute target-http-proxies create http-lb-proxy \
    --url-map=http-lb

gcloud compute forwarding-rules create http-frontend \
    --global \
    --target-http-proxy=http-lb-proxy \
    --ports=80

# create stress tester

# gcloud compute instances create ddos-attack --zone=us-east1-b --machine-type=f1-micro --subnet=default --metadata=startup-script-url=gs://acg-gcloud-course-resources/install-apache-script.sh --boot-disk-device-name=stress-test

gcloud compute instances create ddos-attack --zone=us-east5-a --machine-type=e2-micro --subnet=default --metadata=startup-script-url=gs://acg-gcloud-course-resources/install-apache-script.sh --boot-disk-device-name=stress-test

# Obtain frontend IP address and assign to variable $FRONTEND

for FRONTEND in $(gcloud compute forwarding-rules describe http-frontend --format="get(IPAddress)" --global)
do
  gcloud compute forwarding-rules describe http-frontend --format="get(IPAddress)" --global
done

clear

echo -------------------------------
echo "Script complete, wait about 5 minutes for your load balancer to initialize, then access frontend address in a new tab"
echo "Your load balancer frontend IP address is" $FRONTEND
echo "If you receive an error for the website, wait a few more minutes and try again"
echo -------------------------------