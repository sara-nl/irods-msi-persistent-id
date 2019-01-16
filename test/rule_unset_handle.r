myRule {
       msiPidUnsetHandle(*handle, *key);
       writeLine("stdout", "*handle");
}

INPUT *handle="", *key="", *mvalue=""
OUTPUT ruleExecOut
