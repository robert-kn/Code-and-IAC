# Copy the data in the file ‘web-server-access-log.txt.gz’ to the table ‘access_log’ in the PostgreSQL database ‘template1’.

wget https://cf-courses-data.s3.us.cloud-object-storage.appdomain.cloud/IBM-DB0250EN-SkillsNetwork/labs/Bash%20Scripting/ETL%20using%20shell%20scripting/web-server-access-log.txt.gz

# decompress the gzip compressed file

gunzip web-server-access-log.txt.gz

# extract first four coumns : timestamp, latitude, longitude and visitorid to a separate file.

cut -d "#" -f1,2,3,4 web-server-access-log.txt > extracted.txt

# convert the file into a csv file

tr "#" "," < extracted.txt > transformed.txt

# Load phase
echo "Loading data"
# Set the PostgreSQL password environment variable.
# Replace <yourpassword> with your actual PostgreSQL password.
export PGPASSWORD=gloNJUjXag9vXy8oCtdKa2lZ;
# Send the instructions to connect to 'template1' and
# copy the file to the table 'users' through command pipeline.
# NOTE: The file comes with a header. So use the ‘HEADER’ option in the ‘COPY’ command.
echo "\c template1;\COPY acces_log  FROM '/home/project/transformed.txt' DELIMITERS ',' CSV HEADER;" | psql --username=postgres --host=172.21.164.150

echo "SELECT * FROM acces_log;" | psql --username=postgres --host=172.21.164.150 template1