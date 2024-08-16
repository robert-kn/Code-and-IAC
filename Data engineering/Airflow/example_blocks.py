imports block example

# import the libraries
from datetime import timedelta
# The DAG object; we'll need this to instantiate a DAG
from airflow.models import DAG

# Operators; you need this to write tasks!
from airflow.operators.bash_operator import BashOperator
from airflow.operators.python import PythonOperator
from airflow.operators.email import EmailOperator


DAG Arguments block example

#defining DAG arguments

# You can override them on a per-task basis during operator initialization
default_args = {
    'owner': 'Your name',
    'start_date': days_ago(0),
    'email': ['youemail@somemail.com'],
    'retries': 1,
    'retry_delay': timedelta(minutes=5),
}


DAG arguments are like the initial settings for the DAG.


DAG definition block example

# define the DAG
dag = DAG(
    dag_id='unique_id_for_DAG',
    default_args=default_args,
    description='A simple description of what the DAG does',
    schedule_interval=timedelta(days=1),
)


Here you are creating a variable named dag by instantiating the DAG class with the following 
parameters:

unique_id_for_DAG is the ID of the DAG. This is what you see on the web console. This is what 
you can use to trigger the DAG using a TriggerDagRunOperator.

You are passing the dictionary default_args, in which all the defaults are defined.

description helps us in understanding what this DAG does.

schedule_interval tells us how frequently this DAG runs. In this case every day. (days=1).


task definitions block example

# define the tasks

# define a task with BashOperator
task1 = BashOperator(
    task_id='unique_task_id',
    bash_command='<some bashcommand>',
    dag=dag,
)

# define a task with PythonOperator
task2 = PythonOperator(
    task_id='bash_task',
    python_callable=<the python function to be called>,
    dag=dag,
)

# define a task with EmailOperator
task3 = EmailOperator(
    task_id='mail_task',
    to='recipient@example.com',
    subject='Airflow Email Operator example',
    html_content='<p>This is a test email sent from Airflow.</p>',
    dag=dag,
)


A task is defined using:

A task_id which is a string that helps in identifying the task
The dag this task belongs to
The actual task to be performed
The bash command it represents in case of BashOperator
The Python callable function in case of a PythonOperator
Details of the sender, subject of the mail and the mail text as HTML in case of EmailOperator


task pipeline block example

# task pipeline
task1 >> task2 >> task3

You can also use upstream and downstream to define the pipeline. For example:

task1.set_downstream(task2)
task3.set_upstream(task2)