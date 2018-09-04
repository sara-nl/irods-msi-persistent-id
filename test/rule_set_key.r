myRule {
       msiPidSet(*path, *key, *value, *result);
       writeLine("stdout", "*result");
}

INPUT *path="/tempZone/home/rods/example.txt"
INPUT *key="MYKEY"
INPUT *value="MYVALUE"
OUTPUT ruleExecOut
