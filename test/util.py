import os
import shutil
import signal
import time
from subprocess import PIPE
from subprocess import Popen
from irods.session import iRODSSession
from irods.rule import Rule
PORT = 5000
PID_FILE = "/var/handle_mock_%d.pid" % PORT
LOG_FILE = "/var/log/handle_mock_%d.log" % PORT


def create_user_and_group(user_name, group_name):
    with iRODSSession(host='localhost',
                      port=1247,
                      user='rods',
                      password='test',
                      zone='tempZone') as session:
        group = session.user_groups.create(group_name)
        user = session.users.create(user_name, 'rodsuser')
        group.addmember(user.name)
        cmd = ["iadmin", "moduser", "tst_user", "password", "test"]
        p = Popen(cmd, stdout=PIPE, stderr=PIPE)
        res, err = p.communicate()
        res = res.rstrip()
        if p.returncode:
            raise RuntimeError(" ".join(cmd) + " failed: " + err)


def delete_user_and_group(user_name, group_name):
    with iRODSSession(host='localhost',
                      port=1247,
                      user='rods',
                      password='test',
                      zone='tempZone') as session:
        session.users.get(user_name).remove()
        session.user_groups.get(group_name).remove()


def get_config_file():
    config_file = os.path.abspath('/etc/irods/irods_pid.json')
    backup_file = "%s.back.%d" % (config_file, os.getpid())
    return (config_file, backup_file)


def exec_rule(rule_file, **kwargs):
    rule_path = os.path.join(os.path.dirname(__file__), rule_file)
    cmd = ['irule', '-F', rule_path]
    cmd += ["*{0}='{1}'".format(k, v.replace('"', '\\"'))
            for k, v in kwargs.items()]
    p = Popen(cmd, stdout=PIPE, stderr=PIPE)
    res, err = p.communicate()
    res = res.rstrip()
    if p.returncode:
        raise RuntimeError(" ".join(cmd) + " failed: " + err)
    return res


def exec_rule_list(rule_file, **kwargs):
    ret = exec_rule(rule_file, **kwargs)
    ret = ret.strip()
    if ret:
        return ret.split('\n')
    else:
        return []


def exec_rule_py(rule_file, user, **kwargs):
    rule_path = os.path.join(os.path.dirname(__file__), rule_file)
    with iRODSSession(host='localhost',
                      port=1247,
                      user=user,
                      password='test',
                      zone='tempZone') as session:
        myrule = Rule(session, rule_path)
        myrule.execute()


def ensure_handle_service():
    if os.path.isfile(PID_FILE):
        pid = ""
        with open(PID_FILE, 'r') as myfile:
            pid = myfile.read().replace('\n', ' ')
        raise Exception("handle process pid file exists: %s (pid: %s)" %
                        (PID_FILE, pid))
    else:
        curr_dir = os.path.dirname(os.path.dirname(__file__))
        progr = os.path.abspath(os.path.join(curr_dir,
                                             'surfsara-handle-client-cpp',
                                             'handle-mockup',
                                             'handle_mock.py'))
        config_file, backup_file = get_config_file()

        src_file = os.path.abspath(os.path.join(curr_dir,
                                                'test',
                                                'config.json.mock'))
        if os.path.isfile(config_file):
            shutil.copyfile(config_file, backup_file)
        shutil.copyfile(src_file, config_file)

        Popen([progr,
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


def stop_handle_service():
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
