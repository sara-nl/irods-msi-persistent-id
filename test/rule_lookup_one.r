myRule {
       msiPidLookupOne(*path, *handle);
       writeLine("stdout", "*handle");
}

INPUT *path="/tempZone/home/rods/example.txt"
OUTPUT ruleExecOut
