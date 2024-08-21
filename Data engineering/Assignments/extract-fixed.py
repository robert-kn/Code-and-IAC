import pandas as pd
extracted_path = "/home/project/extracted"
fixed_width_data = "/home/project/extracted-files/fixed_width_data.csv"

def extract_data_from_fixed_width():
    col_positions = [(58,61),(62,67)]
    
    data = pd.read_fwf(f"{extracted_path}/payment-data.txt", colspecs=col_positions,header=None)
    
    data.to_csv(fixed_width_data, index=False, header=False)

extract_data_from_fixed_width()