acPostProcForPut {
    ON($objPath like "/$rodsZoneClient/home/public/*") {
        writeLine("serverLog", "DBG: PID RULE CREATE");
        register($objPath);
    }
}

acPostProcForCollCreate{
    ON($collName like "/$rodsZoneClient/home/public/*"){
         writeLine("serverLog", "DBG: PID RULE CREATE");
         register($collName);
    }
}

acPostProcForPut { }
acPostProcForCollCreate { }

register(*path) {
    msiPidLookup(*path, *handle);
    writeLine("serverLog", "PID RULE CREATE INFO: Lookup Handle  *handle");
    if(*handle=='') {
        writeLine("serverLog", "PID RULE CREATE INFO: No existing handle, creating handle");
        msiPidCreate(*path, "", *handle);
    }
    else {
        writeLine("serverLog", "PID RULE CREATE INFO: Handle already existed: *handle");
    }
    writeLine("serverLog", "PID RULE CREATE INFO: Creating AVU: IRODS/PID: *handle");
    createAVU("IRODS/PID", *handle, *path);
    writeLine("serverLog", "PID RULE CREATE INFO: Created AVU: IRODS/PID: *handle");
}

createAVU(*key, *value, *path) {
    #Creates a key-value pair and attaches it to a data object or collection
    writeLine("serverLog", "AVU RULE CREATE INFO: Creating AVU: Path: *path");
    msiAddKeyVal(*Keyval,*key, *value);
    #writeKeyValPairs("stdout", *Keyval, " is : ");
    msiGetObjType(*path,*objType);
    writeLine("serverLog", "AVU RULE CREATE INFO: Creating AVU: Object type: *objType");
    msiSetKeyValuePairsToObj(*Keyval, *path, *objType);
}