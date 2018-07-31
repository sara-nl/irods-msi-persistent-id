myRule {
       msi_pid_get(*path, *jsonPath, *result);
       writeLine("stdout", "*result");
}

INPUT *path="/tempZone/home/rods/example.txt"
INPUT *jsonPath="IRODS_URL"
OUTPUT ruleExecOut
