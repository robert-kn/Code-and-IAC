import requests

destination = "/home/project/"
download_path = "tolldata.tgz"
extracted_path = "/home/project/extracated"

def download_dataset():
    url = "https://cf-courses-data.s3.us.cloud-object-storage.appdomain.cloud/IBM-DB0250EN-SkillsNetwork/labs/Final%20Assignment/tolldata.tgz"
    r = requests.get(url)
    open(f"{destination}/{download_path}", "wb").write(r.content)
    if r.status_code == 200:
        print("File downloaded successfully")


download_dataset()
