echo "database" | cut -c1-4 --> You should get the string ‘data’ as output.

echo "database" | cut -c5-8 --> You should get the string ‘base’ as output.

echo "database" | cut -c1,5 --> You get the output : ‘db’

We can extract a specific column/field from a delimited text file, by mentioning

the delimiter using the -d option, or
the field number using the -f option.

cut -d":" -f1 /etc/passwd --> The command extracts usernames (the first field) from /etc/passwd.

cut -d":" -f1,3,6 /etc/passwd --> The command extracts multiple fields 1st, 3rd, and 6th (username, userid, and home directory) from /etc/passwd.

cut -d":" -f3-6 /etc/passwd --> The command extracts a range of fields 3rd to 6th (userid, groupid, user description and home directory) from /etc/passwd.

tr is a filter command used to translate, squeeze, and/or delete characters.

echo "Shell Scripting" | tr "[a-z]" "[A-Z]" --> The command translates all lower case alphabets to upper case.

echo "Shell Scripting" | tr "[:lower:]" "[:upper:]" --> same as above

echo "Shell Scripting" | tr  "[A-Z]" "[a-z]" --> The command translates all upper case alphabets to lower case.

The -s option replaces a sequence of a repeated characters with a single occurrence of that character.

The command below replaces repeat occurrences of ‘space’ in the output of ps command with one ‘space’.

ps | tr -s " "

In the above example, the space character within quotes can be replaced with the following : "[\:space\:]".

We can delete specified characters using the -d option.

The command below deletes all digits.

echo "My login pin is 5634" | tr -d "[:digit:]" --> The output will be : ‘My login pin is’


\c template1 --> connect to a database name `template1` once connected to the database via the client

create table users(username varchar(50),userid int,homedirectory varchar(100));

If the table is created successfully, you will get the message below.

CREATE TABLE


To load data from a shell script, you will use the psql client utility in a non-interactive 
manner. This is done by sending the database commands through a command pipeline to psql with 
the help of echo command.

PostgreSQL command to copy data from a CSV file to a table is COPY.

The basic structure of the command which we will use in our script is,

COPY table_name FROM 'filename' DELIMITERS 'delimiter_character' FORMAT;


echo "\c template1;\COPY users  FROM '/home/project/transformed-data.csv' DELIMITERS ',' CSV;" | psql --username=postgres --host=localhost



