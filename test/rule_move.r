myRule {
       msiPidMove(*pathOld, *pathNew, *handle);
       writeLine("stdout", "*handle");
}

INPUT *pathOld="/tempZone/home/rods/example.txt", *pathNew="/tempZone/home/rods/example2.txt"
OUTPUT ruleExecOut
