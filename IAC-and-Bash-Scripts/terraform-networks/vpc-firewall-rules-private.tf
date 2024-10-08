# allow ssh
resource "google_compute_firewall" "private-allow-ssh" {
  name    = "${google_compute_network.private-vpc.name}-allow-ssh"
  network = google_compute_network.private-vpc.name
  allow {
    protocol = "tcp"
    ports    = ["22"]
  }
  source_ranges = [
    "10.1.1.0/24"
  ]
  target_tags = ["allow-ssh"]
}

# allow rdp
resource "google_compute_firewall" "private-allow-rdp" {
  name    = "${google_compute_network.private-vpc.name}-allow-rdp"
  network = google_compute_network.private-vpc.name
  allow {
    protocol = "tcp"
    ports    = ["3389"]
  }
  source_ranges = [
    "10.1.1.0/24"
  ]
  target_tags = ["allow-rdp"]
}

# allow ping only from everywhere
resource "google_compute_firewall" "private-allow-ping" {
  name    = "${google_compute_network.private-vpc.name}-allow-ping"
  network = google_compute_network.private-vpc.name
  allow {
    protocol = "icmp"
  }
  source_ranges = [
    "10.1.1.0/24"
  ]
}
