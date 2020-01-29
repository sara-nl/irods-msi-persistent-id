#!/bin/bash
cmd=$1
shift

/app/wait_for_pg.sh | exit 8

set -x

_RODS_PASSWORD=${RODS_PASSWORD:-rods}
_IRODS_DB_PASSWORD=${IRODS_DB_PASSWORD:-irods}
_MARA_PASSWORD=${MARA_PASSWORD:-mara}
_AYUB_PASSWORD=${AYUB_PASSWORD:-ayub}

if [ -e  /var/lib/irods/iRODS/Vault/home ]
then
    echo "irods already installed"
    /etc/init.d/irods start
else
    cp /app/setup_answers.txt /app/setup_answers.txt.back
    sed -i s/__RODS_PASSWORD__/$_RODS_PASSWORD/g /app/setup_answers.txt
    sed -i s/__IRODS_DB_PASSWORD__/$_IRODS_DB_PASSWORD/g /app/setup_answers.txt
    if [ -f /etc/irods/service_account.config ]
    then
        echo 'setup without service account'
        /var/lib/irods/packaging/setup_irods.sh <  /app/setup_answers.txt &> /app/setup_irods.log &
    else
        ( echo irods
          echo irods
          cat /app/setup_answers.txt
        ) | /var/lib/irods/packaging/setup_irods.sh &> /app/setup_irods.log &
    fi
fi

echo "wait for irods"
echo "--------------"
/app/wait_for_irods.sh

curl icat.irods:1247

chmod a+r /var/lib/irods/VERSION.json

echo "init users"
echo "--------------"
echo $_RODS_PASSWORD | iinit
/app/init_users.sh


# execute command
if [ -z "$cmd" ]
then
    /app/sleep.sh
else
    $cmd $@
fi
