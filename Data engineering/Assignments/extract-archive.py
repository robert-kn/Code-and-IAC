import shutil

# destination = "/home/project/"
file_name = "tolldata.tgz"
# extracted_path = "/home/project/extracted"
destination = "./download"
extracted_path = "./extracted"

def untar_dataset():
    shutil.unpack_archive(f"{destination}/{file_name}", f"{extracted_path}", "gztar")

untar_dataset()