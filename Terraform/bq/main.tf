resource "google_compute_network" "producer_vpc" {
  project                 = var.project_id
  name                    = "producer-net"
  auto_create_subnetworks = false
  # depends_on = [ google_project_service.gcp_services ]
}

resource "google_compute_subnetwork" "prod-subnet" {
  name          = "prod-subnet"
  project       = var.project_id
  region        = var.region
  network       = google_compute_network.producer_vpc.id
  ip_cidr_range = "10.20.1.0/28"
  depends_on    = [google_compute_network.producer_vpc]

  log_config {
    aggregation_interval = "INTERVAL_15_MIN"
    flow_sampling        = 0.1
    metadata             = "INCLUDE_ALL_METADATA"
  }
}

# resource "google_project_service" "gcp_services" {
#   for_each = toset(var.gcp_service_list)
#   project = var.project_id
#   service = each.key
#   disable_on_destroy = true
#   disable_dependent_services = true
# }

resource "google_bigquery_dataset" "dbt-dataset" {
  dataset_id                  = "dbt_practice"
  location                    = "US"
  project = var.project_id
  depends_on = [ google_compute_network.producer_vpc, google_compute_subnetwork.prod-subnet ]
}
