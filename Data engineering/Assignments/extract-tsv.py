extracted_path = "./extracted"
tsv_data = "./processed-files/tsv_data.csv"

def extract_data_from_tsv():
    with open(f"{extracted_path}/tollplaza-data.tsv", 'r') as infile, open(f"{tsv_data}", 'w') as outfile:
        for line in infile:
            fields = line.split('\t')
            if len(fields) > 6:
                field_1 = fields[4]
                field_2 = fields[5]
                field_3 = fields[6]
                outfile.write(field_1 + "," + field_2 + "," + field_3 + "\n")

extract_data_from_tsv()