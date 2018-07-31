myRule {
       msi_pid_update(*pathOld, *pathNew, *handle);
       writeLine("stdout", "*handle");
}

INPUT *pathOld="/tempZone/home/rods/example2.txt"
INPUT *pathNew="/tempZone/home/rods/example.txt"
OUTPUT ruleExecOut
