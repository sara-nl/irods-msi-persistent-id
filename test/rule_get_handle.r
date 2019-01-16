myRule {
       msiPidGetHandle(*handle, *key, *result);
       writeLine("stdout", *result)
}

INPUT *handle="",*key=""
OUTPUT ruleExecOut
