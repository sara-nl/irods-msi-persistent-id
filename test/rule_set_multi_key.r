myRule {
       *key=list("MYKEY", "MYVALUE_2", "MYKEY_2", "VALUE_2")
       msiPidSet(*path, *key, "", *result);
       writeLine("stdout", "*result");
}

INPUT *path="/tempZone/home/rods/example.txt"
OUTPUT ruleExecOut
