# Deploy resources using terraform

resource "google_compute_network" "producer_vpc" {
  project                 = var.project_id
  name                    = "producer-net"
  auto_create_subnetworks = false
}

resource "google_compute_network" "consumer_vpc" {
  project                 = var.project_id
  name                    = "consumer-net"
  auto_create_subnetworks = false
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

resource "google_compute_subnetwork" "lion-subnet-1" {
  name          = "lion-subnet-1"
  project       = var.project_id
  region        = var.region
  network       = google_compute_network.consumer_vpc.id
  ip_cidr_range = "192.168.20.0/28"
  depends_on    = [google_compute_network.consumer_vpc]

  log_config {
    aggregation_interval = "INTERVAL_15_MIN"
    flow_sampling        = 0.1
    metadata             = "INCLUDE_ALL_METADATA"
  }
}

resource "google_compute_subnetwork" "tiger-subnet-1" {
  name          = "tiger-subnet-1"
  project       = var.project_id
  region        = var.region
  network       = google_compute_network.consumer_vpc.id
  ip_cidr_range = "192.168.30.0/28"
  depends_on    = [google_compute_network.consumer_vpc]

  log_config {
    aggregation_interval = "INTERVAL_15_MIN"
    flow_sampling        = 0.1
    metadata             = "INCLUDE_ALL_METADATA"
  }
}

resource "google_compute_subnetwork" "cosmo-subnet-1" {
  name          = "cosmo-subnet-1"
  project       = var.project_id
  region        = var.region
  network       = google_compute_network.consumer_vpc.id
  ip_cidr_range = "192.168.40.0/28"
  depends_on    = [google_compute_network.consumer_vpc]

  log_config {
    aggregation_interval = "INTERVAL_15_MIN"
    flow_sampling        = 0.1
    metadata             = "INCLUDE_ALL_METADATA"
  }
}


resource "google_compute_subnetwork" "netattach-subnet" {
  name          = "netattach-subnet"
  project       = var.project_id
  region        = var.region
  network       = google_compute_network.consumer_vpc.id
  ip_cidr_range = "192.168.10.0/28"
  depends_on    = [google_compute_network.consumer_vpc]

  log_config {
    aggregation_interval = "INTERVAL_15_MIN"
    flow_sampling        = 0.1
    metadata             = "INCLUDE_ALL_METADATA"
  }
}

resource "google_compute_router" "cloud-router-for-nat" {
  name    = "cloud-router-for-nat"
  network = google_compute_network.consumer_vpc.id
  region  = var.region
  project = var.project_id
  depends_on = [google_compute_network.consumer_vpc, google_compute_subnetwork.cosmo-subnet-1,
  google_compute_subnetwork.lion-subnet-1, google_compute_subnetwork.netattach-subnet,
  google_compute_subnetwork.tiger-subnet-1, google_compute_firewall.ssh-iap-consumer]
  # depends_on = [google_compute_network.consumer_vpc, google_compute_subnetwork.cosmo-subnet-1, google_compute_subnetwork.lion-subnet-1,
  # google_compute_subnetwork.netattach-subnet, google_compute_subnetwork.tiger-subnet-1]
}

resource "google_compute_router_nat" "cloud-nat-eu-west2" {
  name                               = "cloud-nat-eu-west2"
  project                            = var.project_id
  router                             = google_compute_router.cloud-router-for-nat.name
  region                             = google_compute_router.cloud-router-for-nat.region
  nat_ip_allocate_option             = "AUTO_ONLY"
  source_subnetwork_ip_ranges_to_nat = "ALL_SUBNETWORKS_ALL_IP_RANGES"

  log_config {
    enable = true
    filter = "ERRORS_ONLY"
  }
  depends_on = [google_compute_router.cloud-router-for-nat]
}

resource "google_compute_firewall" "ssh-iap-consumer" {
  name    = "ssh-iap-consumer"
  network = google_compute_network.consumer_vpc.id
  project = var.project_id

  allow {
    protocol = "tcp"
    ports    = ["22"]
  }

  direction     = "INGRESS"
  source_ranges = ["35.235.240.0/20"]
  depends_on    = [google_compute_network.consumer_vpc, google_compute_subnetwork.cosmo-subnet-1,
  google_compute_subnetwork.lion-subnet-1, google_compute_subnetwork.netattach-subnet,
  google_compute_subnetwork.tiger-subnet-1]
}

resource "google_compute_firewall" "ssh-iap-producer" {
  name    = "ssh-iap-producer"
  network = google_compute_network.producer_vpc.id
  project = var.project_id

  allow {
    protocol = "tcp"
    ports    = ["22"]
  }

  direction     = "INGRESS"
  source_ranges = ["35.235.240.0/20"]
  depends_on    = [google_compute_network.producer_vpc]
}

resource "google_compute_network_attachment" "psc-network-attachment" {
  name                  = "psc-network-attachment"
  connection_preference = "ACCEPT_MANUAL"
  project               = var.project_id
  subnetworks           = [google_compute_subnetwork.netattach-subnet.self_link]
  region                = var.region
  producer_accept_lists = [var.project_id]
  depends_on            = [google_compute_subnetwork.netattach-subnet]
}

# resource "google_compute_instance" "bear" {
#   name           = "bear"
#   machine_type   = "e2-micro"
#   zone           = var.zone
#   project        = var.project_id
#   desired_status = "RUNNING"
#   can_ip_forward = true
#   depends_on     = [google_compute_subnetwork.prod-subnet, google_compute_firewall.ssh-iap-producer ]

#   boot_disk {
#     initialize_params {
#       image = "debian-cloud/debian-11"
#     }
#   }

#   network_interface {
#     network    = google_compute_network.producer_vpc.id
#     subnetwork = google_compute_subnetwork.prod-subnet.id
#   }

#   network_interface {
#     network_attachment = google_compute_network_attachment.psc-network-attachment.id
#   }
# }

# resource "google_compute_instance" "lion" {
#   name           = "lion"
#   machine_type   = "e2-micro"
#   zone           = var.zone
#   project        = var.project_id
#   desired_status = "RUNNING"
#   depends_on     = [google_compute_subnetwork.lion-subnet-1, google_compute_firewall.allow-ingress,
#   google_compute_firewall.allow-limited-egress-to-lion, google_compute_firewall.deny-all-egress,
#   google_compute_firewall.ssh-iap-consumer, google_compute_firewall.vm-subnet-allow-ingress, 
#   google_compute_router.cloud-router-for-nat]

#   boot_disk {
#     initialize_params {
#       image = "debian-cloud/debian-11"
#     }
#   }

#   network_interface {
#     network    = google_compute_network.consumer_vpc.id
#     subnetwork = google_compute_subnetwork.lion-subnet-1.id
#   }

#   metadata_startup_script = file("./lion_startup.sh")
# }

# resource "google_compute_instance" "cosmo" {
#   name           = "cosmo"
#   machine_type   = "e2-micro"
#   zone           = var.zone
#   project        = var.project_id
#   desired_status = "RUNNING"
#   depends_on     = [google_compute_subnetwork.cosmo-subnet-1, google_compute_firewall.allow-ingress,
#   google_compute_firewall.allow-limited-egress-to-lion, google_compute_firewall.deny-all-egress,
#   google_compute_firewall.ssh-iap-consumer, google_compute_firewall.vm-subnet-allow-ingress,
#   google_compute_router.cloud-router-for-nat]

#   boot_disk {
#     initialize_params {
#       image = "debian-cloud/debian-11"
#     }
#   }

#   network_interface {
#     network    = google_compute_network.consumer_vpc.id
#     subnetwork = google_compute_subnetwork.cosmo-subnet-1.id
#   }

#   metadata_startup_script = file("./cosmo_startup.sh")
# }

# resource "google_compute_instance" "tiger" {
#   name           = "tiger"
#   machine_type   = "e2-micro"
#   zone           = var.zone
#   project        = var.project_id
#   desired_status = "RUNNING"
#   depends_on     = [google_compute_subnetwork.tiger-subnet-1, google_compute_firewall.allow-ingress,
#   google_compute_firewall.allow-limited-egress-to-lion, google_compute_firewall.deny-all-egress,
#   google_compute_firewall.ssh-iap-consumer, google_compute_firewall.vm-subnet-allow-ingress,
#   google_compute_router.cloud-router-for-nat]

#   boot_disk {
#     initialize_params {
#       image = "debian-cloud/debian-11"
#     }
#   }

#   network_interface {
#     network    = google_compute_network.consumer_vpc.id
#     subnetwork = google_compute_subnetwork.tiger-subnet-1.id
#   }

#   metadata_startup_script = file("./tiger_startup.sh")
# }

# resource "google_compute_firewall" "allow-limited-egress-to-lion" {
#   name     = "allow-limited-egress-to-lion"
#   network  = google_compute_network.consumer_vpc.id
#   project  = var.project_id
#   priority = "1000"

#   log_config {
#     metadata = "INCLUDE_ALL_METADATA"
#   }

#   allow {
#     protocol = "all"
#   }

#   direction          = "EGRESS"
#   source_ranges      = ["192.168.10.0/28"]
#   destination_ranges = ["192.168.20.0/28"]
#   depends_on         = [google_compute_network.consumer_vpc]
# }


# resource "google_compute_firewall" "allow-ingress" {
#   name     = "allow-ingress"
#   network  = google_compute_network.consumer_vpc.id
#   project  = var.project_id
#   priority = "1000"

#   log_config {
#     metadata = "INCLUDE_ALL_METADATA"
#   }

#   allow {
#     protocol = "all"
#   }

#   direction     = "INGRESS"
#   source_ranges = ["192.168.10.0/28"]
#   depends_on    = [google_compute_network.consumer_vpc]
# }

# resource "google_compute_firewall" "deny-all-egress" {
#   name     = "deny-all-egress"
#   network  = google_compute_network.consumer_vpc.id
#   project  = var.project_id
#   priority = "65534"

#   log_config {
#     metadata = "INCLUDE_ALL_METADATA"
#   }

#   allow {
#     protocol = "all"
#   }

#   direction          = "EGRESS"
#   source_ranges      = ["192.168.10.0/28"]
#   destination_ranges = ["0.0.0.0/0"]
#   depends_on         = [google_compute_network.consumer_vpc]
# }

# resource "google_compute_firewall" "vm-subnet-allow-ingress" {
#   name     = "vm-subnet-allow-ingress"
#   network  = google_compute_network.consumer_vpc.id
#   project  = var.project_id
#   priority = "1000"

#   log_config {
#     metadata = "INCLUDE_ALL_METADATA"
#   }

#   allow {
#     protocol = "all"
#   }

#   direction          = "INGRESS"
#   source_ranges      = ["192.168.40.0/28"]
#   destination_ranges = ["192.168.10.0/28"]
#   depends_on         = [google_compute_network.consumer_vpc]
# }