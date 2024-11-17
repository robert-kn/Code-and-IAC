"""
When run as an airflow job, this python script will download a gzip compressed tar file, extract it's contents
and proceed to retrieve particular columns from files as described by the functions that commence with extract 
below in the code. The files are then combined into one before one of the columns characters are converted to
uppercase. This was the final project from the coursera course titled ETL and Data Pipelines with Shell, Airflow and Kafka.
The downloaded tar file can be found in the downloads folder with the name tolldata.tgz

"""
from datetime import timedelta
from airflow.models import DAG
from airflow.operators.python import PythonOperator

from airflow.utils.dates import days_ago
import requests
import shutil
import pandas as pd

destination = "/home/project/airflow/dags/python_etl/staging"
download_path = "tolldata.tgz"
extracted_path = "/home/project/airflow/dags/python_etl/staging/extracted"
csv_data = "/home/project/airflow/dags/python_etl/staging/csv_data.csv"
tsv_data = "/home/project/airflow/dags/python_etl/staging/tsv_data.csv"
fixed_width_data = "/home/project/airflow/dags/python_etl/staging/fixed_width_data.csv"
combined_extract_file = "/home/project/airflow/dags/python_etl/staging/extracted_data.csv"
transformed_file = "/home/project/airflow/dags/python_etl/staging/transformed_data.csv"

def download_dataset():
    url = "https://cf-courses-data.s3.us.cloud-object-storage.appdomain.cloud/IBM-DB0250EN-SkillsNetwork/labs/Final%20Assignment/tolldata.tgz"
    r = requests.get(url)
    open(f"{destination}/{download_path}", "wb").write(r.content)
    if r.status_code == 200:
        print("File downloaded successfully")

def untar_dataset():
    shutil.unpack_archive(f"{destination}/{download_path}", extracted_path, "gztar") 

def extract_data_from_csv():
    with open(f"{extracted_path}/vehicle-data.csv", "r") as infile, open(csv_data, "w") as outfile:
        for line in infile:
            fields = line.split(',')
            if len(fields) >= 5:
                field_1 = fields[0]
                field_2 = fields[1]
                field_3 = fields[2]
                field_4 = fields[3]
                outfile.write(field_1 + "," + field_2 + "," + field_3 + "," + field_4 + "\n")

def extract_data_from_tsv():
    with open(f"{extracted_path}/tollplaza-data.tsv", "r") as infile, open(tsv_data, "w") as outfile:
        for line in infile:
            fields = line.split('\t')
            if len(fields) > 6:
                field_1 = fields[4]
                field_2 = fields[5]
                field_3 = fields[6]
                outfile.write(field_1 + "," + field_2 + "," + field_3 + "\n")

def extract_data_from_fixed_width():
    col_positions = [(58,61),(62,67)]
    data = pd.read_fwf(f"{extracted_path}/payment-data.txt", colspecs=col_positions,header=None)
    data.to_csv(fixed_width_data, index=False, header=False)

def consolidate_data():
    df_vehicle = pd.read_csv(csv_data)
    df_toll = pd.read_csv(tsv_data)
    df_fixed_width = pd.read_csv(fixed_width_data)
    concatenated_df = pd.concat([df_vehicle, df_toll, df_fixed_width], axis=1)
    concatenated_df.to_csv(combined_extract_file, index=False)

def transform_data():
    df = pd.read_csv(combine_extract_file, header=None)
    df.iloc[:,3] = df.iloc[:,3].apply(lambda x: x.upper())
    df.to_csv(transformed_file, index=False, header=False)

default_args = {
    "owner": "your name",
    "start_date": days_ago(0),
    "email": "your email",
    "retries": 1,
    "retry_delay": timedelta(minutes=5),
}

dag = DAG(
    "ETL_toll_data",
    default_args=default_args,
    description="Apache Airflow Final Assignment",
    schedule_interval=timedelta(days=1),
)

# Define the task named download to call the `download_dataset` function
download_data = PythonOperator(
    task_id='download',
    python_callable=download_dataset,
    dag=dag,
)

unzip_data = PythonOperator(
    task_id='unzip',
    python_callable=untar_dataset,
    dag=dag,
)

extract_data_from_csv = PythonOperator(
    task_id='extractCsv',
    python_callable=extract_data_from_csv,
    dag=dag,
)

extract_data_from_tsv = PythonOperator(
    task_id='extractTsv',
    python_callable=extract_data_from_tsv,
    dag=dag,
)

extract_data_from_fixed_width = PythonOperator(
    task_id='extractFixedWidth',
    python_callable=extract_data_from_fixed_width,
    dag=dag,
)

consolidate_data = PythonOperator(
    task_id='consolidateData',
    python_callable=consolidate_data,
    dag=dag,
)

transform_data = PythonOperator(
    task_id='transformData',
    python_callable=transform_data,
    dag=dag,
)

# Task pipeline
download_data >> unzip_data >> extract_data_from_csv >> extract_data_from_tsv >> extract_data_from_fixed_width >> consolidate_data >> transform_data