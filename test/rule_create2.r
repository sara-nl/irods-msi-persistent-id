myRule {
       *key_values = list("custom_key1", "custom_value1", "custom_key2", "custom_value2");
       # or *key_values = ""
       msiPidCreate(*path,
                    *key_values,
                    *handle);
       writeLine("stdout", "*handle");
}

INPUT *path="/tempZone/home/rods/example42.txt"
OUTPUT ruleExecOut
