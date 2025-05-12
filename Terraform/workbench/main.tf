resource "google_project_service" "gcp_services" {
  for_each = toset(var.gcp_service_list)
  project = var.project_id
  service = each.key
  disable_on_destroy = true
  disable_dependent_services = true
}

resource "google_compute_network" "wb_vpc" {
  project                 = var.project_id
  name                    = "wb-net"
  auto_create_subnetworks = false
}

resource "google_compute_subnetwork" "wb-subnet" {
  name          = "wb-subnet"
  project       = var.project_id
  region        = var.region
  network       = google_compute_network.wb_vpc.id
  ip_cidr_range = "10.20.1.0/28"
  depends_on    = [google_compute_network.producer_vpc]

  log_config {
    aggregation_interval = "INTERVAL_15_MIN"
    flow_sampling        = 0.1
    metadata             = "INCLUDE_ALL_METADATA"
  }
}

data "google_compute_default_service_account" "default" {
  depends_on =  [google_project_service.gcp_services]
}

resource "google_project_iam_member" "storage_admin" {
    project = var.project_id
    role = "roles/storage.admin"
  member = data.google_compute_default_service_account.default.member
}

resource "google_project_iam_member" "vertex_ai_agent" {
  project = var.project_id
  role = "role"
}