extracted_path = "/home/project/extracted"
csv_data = "/home/project/extracted-files/csv_data.csv"

def extract_data_from_csv():
    with open(f"{extracted_path}/vehicle-data.csv", 'r') as infile, open(f"{csv_data}", 'w') as outfile:
        for line in infile:
            fields = line.split(',')
            if len(fields) >= 5:
                field_1 = fields[0]
                field_2 = fields[1]
                field_3 = fields[2]
                field_4 = fields[3]
                outfile.write(field_1 + "," + field_2 + "," + field_3 + "," + field_4 + "\n")

extract_data_from_csv()