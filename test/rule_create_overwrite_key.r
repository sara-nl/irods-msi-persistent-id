# this rule fails because it overwrites default fields
myRule {
       *key_values = list("URL2", "dont_do_that", "IRODS/WEBDAV_URL", "custom_value2");
       # or *key_values = ""
       msiPidCreate(*path,
                    *key_values,
                    *handle);
       writeLine("stdout", "*handle");
}

INPUT *path="/tempZone/home/rods/example2.txt"
OUTPUT ruleExecOut
