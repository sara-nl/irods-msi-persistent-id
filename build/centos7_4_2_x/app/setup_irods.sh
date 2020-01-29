#!/bin/bash
cmd=$1
shift

/app/wait_for_pg.sh | exit 8

set -x

_RODS_PASSWORD=${RODS_PASSWORD:-rods}
_IRODS_DB_PASSWORD=${IRODS_DB_PASSWORD:-irods}

if [ -e  /var/lib/irods/iRODS/Vault/home ]
then
    echo "irods already installed"
    /etc/init.d/irods start
else
    cp /app/setup_answers.txt /app/setup_answers.txt.back
    sed -i s/__RODS_PASSWORD__/$_RODS_PASSWORD/g /app/setup_answers.txt
    sed -i s/__IRODS_DB_PASSWORD__/$_IRODS_DB_PASSWORD/g /app/setup_answers.txt
    python /var/lib/irods/scripts/setup_irods.py < /app/setup_answers.txt &> /app/setup_irods.log &
fi

echo "wait for irods"
echo "--------------"
/app/wait_for_irods.sh

curl icat.irods:1247

echo "init users"
echo "--------------"
echo $_RODS_PASSWORD | iinit
/app/init_users.sh

chmod a+r /var/lib/irods/VERSION.json

# execute command
if [ -z "$cmd" ]
then
    /app/sleep.sh
else
    $cmd $@
fi
