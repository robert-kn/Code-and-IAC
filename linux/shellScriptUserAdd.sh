# this script will add users to a linux box

USERFILE=$1

if ["$USERFILE" = ""]
   then
     echo "Please specify an input file"
     exit 10
elif test -s $USERFILE
   then
     for user in `cat $USERFILE`
     do
         echo "Creating the "$user" user..."
              useradd -m $user; echo "$user:linux" | chpasswd
     done
     exit 20
else
   echo "invalid input file specified!"
   exit 30
fi