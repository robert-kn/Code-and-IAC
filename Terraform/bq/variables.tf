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

# variable "gcp_service_list" {
#   type = list(string)
#   default = [
#     "bigquery.googleapis.com",
#     "compute.googleapis.com"
#   ]
# }