myRule {
       if(strlen(*key_values_inp) > 0)
       {
          *key_values = split(*key_values_inp, ",");
       }
       else
       {
          *key_values = "";
       }
       msiPidCreate(*path,
                    *key_values,
                    *handle);
       writeLine("stdout", *handle)
}

INPUT *path="/tempZone/home/rods/example.txt", *key_values_inp=""
OUTPUT ruleExecOut

