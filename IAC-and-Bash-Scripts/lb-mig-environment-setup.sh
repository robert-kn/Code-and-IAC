# Create firewall rules allowing http and load balancer/health check access

gcloud compute firewall-rules create http-allow --direction=INGRESS --priority=1000 --network=default --action=ALLOW --rules=tcp:80 --source-ranges=0.0.0.0/0 --target-tags=http-server

gcloud compute firewall-rules create health-check-allow --direction=INGRESS --priority=1000 --network=default --action=ALLOW --rules=tcp --source-ranges=130.211.0.0/22,35.191.0.0/16 --target-tags=http-server

# create version 1 template

gcloud compute instance-templates create template-v1 --machine-type=f1-micro --metadata=startup-script-url=gs://la-gcloud-course-resources/network-engineer/mig-startup-v1.sh --tags=http-server --boot-disk-device-name=template-v1

# create version 2 template

gcloud compute instance-templates create template-v2 --machine-type=f1-micro --metadata=startup-script-url=gs://la-gcloud-course-resources/network-engineer/mig-startup-v2.sh --tags=http-server --boot-disk-device-name=template-v2

# create health check and groups from version 1 template

gcloud compute health-checks create tcp "health-check" --timeout "5" --check-interval "10" --unhealthy-threshold "3" --healthy-threshold "2" --port "80"

gcloud beta compute instance-groups managed create us-central1-group --base-instance-name=us-central1-group --template=template-v1 --size=1 --zones=us-central1-b,us-central1-c,us-central1-f --instance-redistribution-type=PROACTIVE --health-check=health-check --initial-delay=300

gcloud beta compute instance-groups managed set-autoscaling "us-central1-group" --region "us-central1" --cool-down-period "60" --max-num-replicas "4" --min-num-replicas "2" --target-cpu-utilization "0.6"

gcloud beta compute instance-groups managed create europe-north1-group --base-instance-name=europe-north1-group --template=template-v1 --size=1 --zones=europe-north1-a,europe-north1-b,europe-north1-c --instance-redistribution-type=PROACTIVE --health-check=health-check --initial-delay=300

gcloud beta compute instance-groups managed set-autoscaling "europe-north1-group" --region "europe-north1" --cool-down-period "60" --max-num-replicas "4" --min-num-replicas "2" --target-cpu-utilization "0.6"


# set names ports for instance groups

gcloud compute instance-groups managed set-named-ports europe-north1-group \
    --named-ports http:80 \
    --region europe-north1

gcloud compute instance-groups managed set-named-ports us-central1-group \
    --named-ports http:80 \
    --region us-central1

# create backend service and add backends

gcloud compute backend-services create http-backend \
    --protocol HTTP \
    --health-checks health-check \
    --global

gcloud compute backend-services add-backend http-backend \
    --balancing-mode=RATE \
    --max-rate-per-instance=50 \
    --capacity-scaler=1 \
    --instance-group=europe-north1-group \
    --instance-group-region=europe-north1 \
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

gcloud compute instances create stress-test --zone=us-east1-b --machine-type=n1-standard-1 --subnet=default --metadata=startup-script-url=gs://la-gcloud-course-resources/install-apache-script.sh --boot-disk-device-name=stress-test

clear

echo -------------------------------
echo "Script complete, wait about 5 minutes for load balancer to initialize, then access frontend address in a new tab"
echo -------------------------------
