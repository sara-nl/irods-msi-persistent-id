import socket
import os
import pprint
from behave import *
from irods.session import iRODSSession
from irods.rule import Rule

def get_session():
    return iRODSSession(host='localhost',
                        port=1247, user='rods',
                        password='test',
                        zone='tempZone')

@given(u'an object {obj} from {source}')
def step_impl(context, obj, source):
    with get_session() as session:
        inp = os.path.join(os.path.dirname(os.path.realpath(__file__)), source)
        session.data_objects.put(inp, obj)

@then(u'wordcount of {obj} is {n}')
def step_impl(context, obj, n):
    RULE = "wc_rule {\n" + \
           " msi_wordcount(*object, *res);\n" + \
           " writeLine(\"stdout\", \"*res\");\n" + \
           "}"
    params = {"*object": '"{0}"'.format(str(obj))}
    with get_session() as session:
        myrule = Rule(session,
                      body=RULE,
                      params=params,
                      output="ruleExecOut")
        res = myrule.execute()
        buf = res.MsParam_PI[0].inOutStruct.stdoutBuf.buf
        # todo better solution here
        buf = str(buf.decode('utf-8', errors='ignore')).strip().replace("\n", "").replace('\0', '')
        assert(int(buf) == int(n))
