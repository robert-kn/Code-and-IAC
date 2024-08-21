import gzip
import shutil

destination = "/home/project/"
download_path = "tolldata.tgz"
extracted_path = "/home/project/extracted"

def untar_dataset():
    with gzip.open(f"{destination}/{download_path}", "rt") as gz:
        data = gz.read()
        print(data)
        #shutil.unpack_archive(data, extracted_path )

untar_dataset()