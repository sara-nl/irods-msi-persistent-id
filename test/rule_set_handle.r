myRule {
       if(strlen(*mvalue) > 0)
       {
          *value = ""
          *key = split(*mvalue, ",");
       }
       msiPidSetHandle(*handle, *key, *value);
       writeLine("stdout", "*handle");
}

INPUT *handle="", *key="MYKEY", *value="MYVALUE", *mvalue=""
OUTPUT ruleExecOut
