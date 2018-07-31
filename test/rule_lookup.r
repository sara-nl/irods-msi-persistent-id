myRule {
       msi_pid_lookup(*path, *handle);
       writeLine("stdout", "*handle");
}

INPUT *path="/tempZone/home/rods/example.txt"
OUTPUT ruleExecOut
