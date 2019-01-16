myRule {
       if(strlen(*mvalue) > 0)
       {
          *value = ""
          *key = split(*mvalue, ",");
       }
       msiPidSet(*path, *key, *value, *result);
       writeLine("stdout", "*result");
}

INPUT *path="/tempZone/home/rods/example.txt", *key="MYKEY", *value="MYVALUE", *mvalue=""
OUTPUT ruleExecOut
