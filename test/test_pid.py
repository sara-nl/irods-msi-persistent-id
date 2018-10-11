##############################################################################
# Copyright 2018, SURFsara
# Author Stefan Wolfsheimer
#
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
##############################################################################
import pytest
import unittest
import os
import subprocess
import signal
import time
import shutil
import re
from irods.session import iRODSSession
from irods.rule import Rule

process = None
PORT = 5000
PID_FILE = "/var/handle_mock_%d.pid" % PORT
LOG_FILE = "/var/log/handle_mock_%d.log" % PORT
PREFIX = "21.T12995"


#########################################################
#
# helper functions
#
#########################################################
def get_config_file():
    config_file = os.path.abspath('/etc/irods/irods_pid.json')
    backup_file = "%s.back.%d" % (config_file, os.getpid())
    return (config_file, backup_file)


def get_session():
    return iRODSSession(host='localhost',
                        port=1247, user='rods',
                        password='test',
                        zone='tempZone')


def get_rule(session, rule_file, **kwargs):
    params = {"*" + k: '"{0}"'.format(v.replace('"', '\\"'))
              for k, v in kwargs.items()}
    rule_path = os.path.join(os.path.dirname(__file__), rule_file)
    return Rule(session,
                rule_file=rule_path,
                params=params)


def exec_rule(session, rule_file, **kwargs):
    rule = get_rule(session, rule_file, **kwargs)
    res = rule.execute()
    return get_return_value(res, 0)


def get_return_value(res, index):
    # from pprint import pprint
    # pprint(res.MsParam_PI) # .MsParam_PI)
    buf = res.MsParam_PI[index].inOutStruct.stdoutBuf.buf
    # todo better solution here
    buf = str(buf.decode('utf-8',
                         errors='ignore')).strip().replace("\n",
                                                           "").replace('\0',
                                                                       '')
    return buf


#########################################################
#
# tests
#
#########################################################

class TestPidMicroServices(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        if os.path.isfile(PID_FILE):
            pid = ""
            with open(PID_FILE, 'r') as myfile:
                pid = myfile.read().replace('\n', ' ')
            raise Exception("handle process pid file exists: %s (pid: %s)" %
                            (PID_FILE, pid))
        else:
            curr_dir = os.path.dirname(os.path.dirname(__file__))
            progr = os.path.abspath(os.path.join(curr_dir,
                                                 'handle-cpp-client',
                                                 'handle-mockup',
                                                 'handle_mock.py'))
            config_file, backup_file = get_config_file()

            src_file = os.path.abspath(os.path.join(curr_dir,
                                                    'handle-cpp-client',
                                                    'config.json.mock'))
            if os.path.isfile(config_file):
                shutil.copyfile(config_file, backup_file)
            shutil.copyfile(src_file, config_file)
            subprocess.Popen([progr,
                              '--port', str(PORT),
                              '--pid_file', PID_FILE],
                             stdout=None,
                             stderr=None)
            total = 0
            while not os.path.isfile(PID_FILE):
                if total == 10:
                    break
                total += 1
                time.sleep(1)
            assert os.path.isfile(PID_FILE)

    @classmethod
    def tearDownClass(cls):
        config_file, backup_file = get_config_file()
        if os.path.isfile(backup_file):
            shutil.copyfile(backup_file, config_file)
        if os.path.isfile(PID_FILE):
            pid = 0
            with open(PID_FILE, 'r') as myfile:
                pid = myfile.read().replace('\n', ' ')
            os.kill(int(pid), signal.SIGKILL)
        else:
            raise Exception("handle process pid file does not exist: %s" %
                            PID_FILE)

    @pytest.mark.run(order=1)
    def test_get_pid_for_undefined_object(self):
        with get_session() as session:
            rule_lookup = get_rule(session, "rule_lookup.r")
            res_lookup = rule_lookup.execute()
            assert get_return_value(res_lookup, 0) == ""
            rule = get_rule(session, "rule_get.r")
            with pytest.raises(Exception):
                rule.execute()

    @pytest.mark.run(order=2)
    def test_pid_life_cycle_object(self):
        with get_session() as session:
            pid = exec_rule(session, "rule_create.r")
            assert re.match(re.escape(PREFIX) + "/.+", pid)
            assert exec_rule(session, "rule_lookup.r") == pid
            assert exec_rule(session, "rule_move.r") is not None
            assert exec_rule(session, "rule_lookup.r") == ""
            assert exec_rule(session, "rule_move_reverse.r") is not None
            assert exec_rule(session, "rule_lookup.r") == pid
            assert (exec_rule(session, "rule_get_irods_url.r") ==
                    "irods://localhost/tempZone/home/rods/example.txt")
            assert exec_rule(session, "rule_get_key.r") == ""
            assert exec_rule(session, "rule_set_key.r") == pid
            assert exec_rule(session, "rule_get_key.r",
                             key="MYKEY") == "MYVALUE"
            assert exec_rule(session, "rule_set_multi_key.r") == pid
            assert exec_rule(session, "rule_get_key.r",
                             key="MYKEY") == "MYVALUE_2"
            assert exec_rule(session, "rule_unset_key.r") == pid
            assert exec_rule(session, "rule_get_key.r") == ""
            assert exec_rule(session, "rule_delete.r") == pid

    @pytest.mark.run(order=3)
    def test_get_deleted_object(self):
        with get_session() as session:
            rule_lookup = get_rule(session, "rule_lookup.r")
            res_lookup = rule_lookup.execute()
            assert get_return_value(res_lookup, 0) == ""
            rule = get_rule(session, "rule_get.r")
            with pytest.raises(Exception):
                rule.execute()
