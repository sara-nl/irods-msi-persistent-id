myRule {
       msiPidLookup(*path, *handles);
       foreach(*handles) {
            writeLine("stdout", *handles)
       }
}

INPUT *path="/tempZone/home/rods/*"
OUTPUT ruleExecOut
