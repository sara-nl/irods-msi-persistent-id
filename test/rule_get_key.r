myRule {
       msiPidGet(*path, *key, *result);
       writeLine("stdout", "*result");
}

INPUT *path="/tempZone/home/rods/example.txt"
INPUT *key="MYKEY"
OUTPUT ruleExecOut
