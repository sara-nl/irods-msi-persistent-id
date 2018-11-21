myRule {
       msiPidLookupKey(*key, "*/tempZone/home/rods/example.txt", *handles);
       foreach(*handles) {
            writeLine("stdout", "*handles");
       }
}

INPUT *key="IRODS/URL"
OUTPUT ruleExecOut
