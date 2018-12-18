#!/bin/bash
/app/wait_for_pg.sh

set -e
set -x

cat /app/setup_answers.txt | python /var/lib/irods/scripts/setup_irods.py
/app/sleep.sh
