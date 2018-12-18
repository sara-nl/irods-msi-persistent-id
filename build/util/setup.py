import os
import sys
import time
import subprocess


def start_docker_composite(volume, cwd):
    env = os.environ.copy()
    env['VOLUME'] = volume
    p = subprocess.Popen(["docker-compose", "up", "--detach"],
                         stdout=sys.stdout,
                         stderr=sys.stderr,
                         cwd=cwd,
                         env=env)
    p.communicate()
    exit_code = p.poll()
    if exit_code != 0:
        raise Exception("docker-compose exited with code %d" % exit_code)
    else:
        print("docker composite has been started")
        print("logs are available via\n")
        print("docker logs -t %s_icat_1" % os.path.basename(cwd))


def has_running_container(cwd):
    p = subprocess.Popen(["docker", "ps", "--format", '{{.Names}}'],
                         stdout=subprocess.PIPE)
    with p.stdout:
        for line in iter(p.stdout.readline, b''):
            if(line.startswith(cwd)):
                return line
    p.wait()
    return None


def check_iinit(cwd):
    cmd = ["docker",
           "exec",
           cwd + "_icat_1",
           "/bin/bash",
           "-c",
           'echo test | iinit; exit $?']
    with open(os.devnull, 'w')  as FNULL:
        p = subprocess.Popen(cmd, stdout=FNULL, stderr=FNULL)
        p.communicate()
        return p.poll() == 0


def shut_down_composite(composite):
    basedir = os.path.dirname(os.path.dirname(os.path.realpath(__file__)))
    cwd = os.path.join(basedir, composite)
    p = subprocess.Popen(["docker-compose", "down"],
                         stdout=sys.stdout,
                         stderr=sys.stderr,
                         cwd=cwd)
    p.communicate()
    exit_code = p.poll()
    if exit_code != 0:
        raise Exception("docker-compose exited with code %d" % exit_code)
    else:
        print("docker composite has been shut down")


def setup_composite(volume, composite, basedir=None):
    if basedir is None:
        basedir = os.path.dirname(os.path.dirname(os.path.realpath(__file__)))
    running = has_running_container(composite)
    if running is None:
        start_docker_composite(volume,
                               os.path.join(basedir, composite))
    else:
        msg = 'There are already running containers for this version: %s.'
        msg += 'Remove them before continuing'
        msg = msg % running
        raise Exception(msg)
    n_trials = 10
    trials = n_trials
    running = False
    while trials > 0 and not running:
        trials -= 1
        print("Attempt to connect to icat server trial %d / %d" %
              (n_trials - trials, n_trials))
        running = check_iinit(composite)
        time.sleep(10)
    if not running:
        raise Exception("failed to start iCat server, check logs with\n"
                        "docker logs -t %s_icat_1" % composite)

    print("iCAT container is ready")
