myRule {
       msiPidLookupKey(*key, *value, *handles);
       foreach(*handles) {
            writeLine("stdout", *handles)
       }
}

INPUT *key="IRODS/URL", *value="http://localhost:80/tempZone/home/rods/example.txt"
OUTPUT ruleExecOut
