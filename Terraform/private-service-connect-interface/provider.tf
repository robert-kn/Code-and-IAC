# The terraform {} block contains Terraform settings, including the required providers Terraform will use to provision your 
# infrastructure. For each provider, the source attribute defines an optional hostname, a namespace, and the provider type. 
# Terraform installs providers from the Terraform Registry by default. In this example configuration, the google provider's 
# source is defined as hashicorp/google, which is shorthand for registry.terraform.io/hashicorp/google.

terraform {
  required_providers {
    google = {
      source  = "hashicorp/google"
      version = "~> 6.29.0"
    }
  }
}
