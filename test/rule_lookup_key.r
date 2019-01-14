myRule {
       msiPidLookupKey(*key, "*/tempZone/home/rods/example.txt", *handles);
       foreach(*handles) {
            writeLine("stdout", "*handles");
            writeLine("stdout", "---0ffaa51c-1804-11e9-9ea9-b76b05597c82---");
       }
}

INPUT *key="IRODS/URL"
OUTPUT ruleExecOut
