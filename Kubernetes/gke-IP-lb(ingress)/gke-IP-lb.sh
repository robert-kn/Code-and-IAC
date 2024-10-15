# I am going to take the custom range cluster initiate 2 deployments, then pair each with a service, and use an 
# **Ingress** object to create an HTTP load balancer.

# The commands to recreate the custom VPC network and GKE cluster are below, as well as the commands used to create 
# and deploy our pods, services, and ingress.

### Create custom VPC network with subnet and secondary IP ranges:

    gcloud compute networks create custom-network --subnet-mode=custom

    gcloud compute networks subnets create subnet-a --network=custom-network --region=us-central1 --range=192.168.25.0/24 --secondary-range=pod-range=10.2.0.0/16,service-range=10.3.0.0/16 --enable-private-ip-google-access
    

### Create GKE cluster using the pod and service IP ranges specified above:

    gcloud beta container clusters create custom-range-cluster --zone "us-central1-a" --enable-ip-alias --network "custom-network" --subnetwork "subnet-a" --cluster-secondary-range-name "pod-range" --services-secondary-range-name "service-range" 

### Get the kubectl credentials for our cluster so we can run commands against it:

    gcloud container clusters get-credentials custom-range-cluster --zone us-central1-a

### Create two `hello world` pod deployments and two clusterIP services to expose both deployments, along with the 
# ingress object. 


### Create and expose deployments:

    # kubectl apply -f hello-world-deployment.yaml

    # kubectl apply -f hello-world-service.yaml

    # kubectl apply -f hello-kubernetes-deployment.yaml

    # kubectl apply -f hello-kubernetes-service.yaml

### Create the ingress, which will set up the HTTP load balancer:

    # kubectl apply -f my-ingress.yaml

### View ingress:

    # kubectl get ingress my-ingress --output yaml