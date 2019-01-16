myRule {
       msiPidMoveHandle(*handle, *pathNew);
       writeLine("stdout", "*handle");
}

INPUT *handle="", *pathNew="/tempZone/home/rods/example2.txt"
OUTPUT ruleExecOut
