import os
import sys
import re
import glob
import subprocess
import argparse


PACKAGE_NAME = "msi-persistent-id"
RPM_ROOT = '/root/rpmbuild'


def determine_git_time_stamp():
    cmd = ["git", "log", "-1", "--date=raw"]
    cwd = os.path.dirname(__file__)
    p = subprocess.Popen(cmd,
                         stdout=subprocess.PIPE,
                         stderr=sys.stderr,
                         cwd=cwd)
    stdout, stderr = p.communicate()
    exit_code = p.poll()
    if exit_code != 0:
        raise Exception(" ".join(cmd) +
                        "exited with code {0}".format(exit_code))
    dtg = 0
    for line in stdout.decode("ascii").split('\n'):
        m = re.match('Date: *([0-9]+)', line)
        if m:
            dtg = m.group(1)
            break
    return dtg


def determine_version(branch=None):
    '''
    determine branch and version
    '''
    if branch is None:
        cmd = ["git", "rev-parse", "--abbrev-ref", "HEAD"]
        cwd = os.path.dirname(__file__)
        p = subprocess.Popen(cmd,
                             stdout=subprocess.PIPE,
                             stderr=sys.stderr,
                             cwd=cwd)
        stdout, stderr = p.communicate()
        exit_code = p.poll()
        if exit_code != 0:
            raise Exception(" ".join(cmd) +
                            "exited with code {0}".format(exit_code))
        rev = stdout.strip().decode('ascii')
    else:
        rev = branch
    tags_match = re.match('^v([0-9]+\\.[0-9]+\\.[0-9]+)$', rev)
    if not tags_match:
        # is a feature branch
        dtg = determine_git_time_stamp()
        return (rev, dtg)
    else:
        # is a release
        return ("", tags_match.group(1))


def extract_irods_version(composite_path):
    composite = os.path.basename(composite_path)
    res = re.match('^centos7_([0-9]+)_([0-9]+)_([0-9]+)$', composite)
    if res is not None:
        return "{0}.{1}.{2}".format(res.group(1),
                                    res.group(2),
                                    res.group(3))


def parse_argments(verbose=False, with_dryrun=False):
    basedir = os.path.dirname(os.path.realpath(__file__))
    repodir = os.path.dirname(basedir)
    pattern = os.path.join(basedir, "centos7_*")
    paths = glob.glob(pattern)
    irods_versions = [extract_irods_version(p)
                      for p in paths
                      if extract_irods_version(p)]

    parser = argparse.ArgumentParser(description='Package microservice in rpm')
    parser.add_argument('--irods-version', choices=irods_versions)
    parser.add_argument('--version',
                        help="""version of the package,
                        if empty determined from git""")
    parser.add_argument('--branch',
                        help="branch of the project")
    parser.add_argument('--project',
                        help="name of the project")
    if with_dryrun:
        parser.add_argument('--dryrun',
                            action="store_true")
    args = parser.parse_args()
    err = False
    if args.irods_version is None:
        print("argument --irods-version required")
        err = True
    if args.version is None:
        (branch, version) = determine_version(branch=args.branch)
    else:
        branch = args.branch
        version = args.version
    if not re.match('^[0-9]+\\.[0-9]+\\.[0-9]+$|[0-9]+$', version):
        print(("invalid version {0}, " +
               "not in the form MAJOR.MINOR.PATCH_LEVEL").format(version))
        err = True
    if err:
        sys.exit(8)
    irods_version = args.irods_version
    container_name = "centos7_{0}_icat_1".format(irods_version.replace('.',
                                                                       '_'))
    rpm_package = "{0}-{1}-{2}.el7.x86_64".format(PACKAGE_NAME,
                                                  version,
                                                  1)
    if container_name.startswith('centos7'):
        create_dir = ['RPMS', 'CentOS', '7']
        if branch:
            create_dir += [branch]
        create_dir += ['irods-{0}'.format(irods_version)]
        rpm_dir = os.path.join(repodir, *create_dir)
        rpm_dir_inside = os.path.join('/build/', *create_dir)

    ret = {
        'package': PACKAGE_NAME,
        'project': args.project,
        'branch': branch,
        'rpm_root': RPM_ROOT,
        'repodir': repodir,
        'irods_version': irods_version,
        'container_name': container_name,
        'docker_exec': ["docker", "exec", container_name],
        'version': version,
        'irods_versions': irods_versions,
        'rpm_package': rpm_package,
        'rpm_dir': rpm_dir,
        'rpm_dir_inside': rpm_dir_inside,
        'dryrun': args.dryrun if with_dryrun else False
    }
    if verbose:
        for k, v in ret.items():
            print("{0:20}: {1}".format(k, v))
    return Config(**ret)


class Config(object):
    def __init__(self, **args):
        for k, v in args.items():
            setattr(self, k, v)
