myRule {
       msi_wordcount(*file, *res);
       writeLine("stdout", "*res");
}

INPUT *file="/tempZone/home/rods/example.txt"
OUTPUT ruleExecOut, *name
