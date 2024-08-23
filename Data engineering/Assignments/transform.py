import pandas as pd
transformed_file = "./processed-files/transformed_data.csv"
combine_extract_file = "./processed-files/combine-extracted.csv"

def transform_data():
    df = pd.read_csv(combine_extract_file, header=None)
    df.iloc[:,3] = df.iloc[:,3].apply(lambda x: x.upper())
    df.to_csv(transformed_file, index=False, header=False)

transform_data()