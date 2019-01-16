myRule {
       msiPidUnset(*path, *key, *result);
       writeLine("stdout", "*result");
}

INPUT *path="/tempZone/home/rods/example.txt", *key="", *mvalue=""
OUTPUT ruleExecOut
