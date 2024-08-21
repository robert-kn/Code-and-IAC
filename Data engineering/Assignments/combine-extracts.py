import pandas as pd

csv_data = "/home/project/extracted-files/csv_data.csv"
fixed_width_data = "/home/project/extracted-files/fixed_width_data.csv"
tsv_data = "/home/project/extracted-files/tsv_data.csv"
final_file = "/home/project/extracted-files/final-data.csv"

def consolidate_data():
    df_vehicle = pd.read_csv(csv_data)
    df_toll = pd.read_csv(tsv_data)
    df_fixed_width = pd.read_csv(fixed_width_data)
    concatenated_df = pd.concat([df_vehicle, df_toll, df_fixed_width], axis=1)
    concatenated_df.to_csv(final_file, index=False)

consolidate_data()