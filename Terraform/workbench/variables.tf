variable "project_id" {
  type = string
}

variable "region" {
  type    = string
  default = "europe-west2"
}

variable "zone" {
  type    = string
  default = "europe-west2-a"
}

variable "machine_type" {
  type    = string
  default = "e2-standard-4"
}

variable "gcp_service_list" {
  type = list(string)
  default = [
    "containerregistry.googleapis.com",
    "aiplatform.googleapis.com",
    "cloudbuild.googleapis.com",
    "cloudfunctions.googleapis.com",
    "iam.googleapis.com",
    "notebooks.googleapis.com"
  ]
}