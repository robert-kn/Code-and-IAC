# Deploy resources using terraform

resource "google_compute_network" "producer_vpc" {
    project = var.project_id
    name = "producer-net"
    auto_create_subnetworks = false
}

resource "google_compute_network" "consumer_vpc" {
    project = var.project_id
    name = "consumer-net"
    auto_create_subnetworks = false
}