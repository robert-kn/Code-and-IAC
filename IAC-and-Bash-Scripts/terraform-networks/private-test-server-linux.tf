# Create Test Server in Public VPC
resource "google_compute_instance" "test-server-linux-private" {
  name         = "private-test-server-linux-${random_id.instance_id.hex}"
  machine_type = "f1-micro"
  zone         = var.gcp_zone_2
  tags         = ["allow-ssh"]

  boot_disk {
    initialize_params {
      image = "debian-cloud/debian-11"
    }
  }

  metadata_startup_script = "sudo apt-get update;"

  network_interface {
    network        = google_compute_network.private-vpc.name
    subnetwork     = google_compute_subnetwork.private-subnet_1.name
    access_config { }
  }
}


output "test-server-linux-private" {
  value = google_compute_instance.test-server-linux-private.name
}

output "test-server-linux-private-external-ip" {
  value = google_compute_instance.test-server-linux-private.network_interface.0.access_config.0.nat_ip
}

output "test-server-linux-private-internal-ip" {
  value = google_compute_instance.test-server-linux-private.network_interface.0.network_ip
}