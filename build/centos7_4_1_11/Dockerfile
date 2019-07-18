FROM centos:7

RUN yum update -y && \
    yum install -y \
    rsync wget less emacs make cmake gcc gcc-c++

RUN rpm -ivh http://dl.fedoraproject.org/pub/epel/epel-release-latest-7.noarch.rpm && \
    rpm --import https://packages.irods.org/irods-signing-key.asc && \
    wget -qO - https://packages.irods.org/renci-irods.yum.repo | \
    tee /etc/yum.repos.d/renci-irods.yum.repo

RUN yum install -y \
    sudo \
    which \
    python-jsonschema \
    python-psutil \
    python-pytest \
    python-requests \
    python-pip \
    python-behave \
    python-devel \
    openssl-devel \
    fuse-libs \
    lsof \
    openssl \
    perl-JSON \
    authd \
    postgresql \
    postgresql-odbc\
    unixODBC \
    boost-devel \
    libcurl-devel \
    supervisor \
    rpm-build \
    rpm-devel \
    rpmlint \
    rpmdevtools \
    coreutils \
    diffutils \
    patch

# Openstack
RUN yum install -y centos-release-openstack-rocky

RUN yum install -y \
    python-swiftclient \
    python2-keystoneclient \
    python-openstackclient


RUN pip install --upgrade pip && \
    pip install flask \
                flask_restful \
                pytest \
                s3cmd \
                jsonschema \
                python-irodsclient

COPY app/odbcinst.ini /etc/odbcinst.ini

################################################################################
#
# irods
#
################################################################################
RUN yum install -y ftp://ftp.renci.org/pub/irods/releases/4.1.11/centos7/irods-icat-4.1.11-centos7-x86_64.rpm &&\
    yum install -y ftp://ftp.renci.org/pub/irods/releases/4.1.11/centos7/irods-database-plugin-postgres-1.11-centos7-x86_64.rpm && \
    yum install -y ftp://ftp.renci.org/pub/irods/releases/4.1.11/centos7/irods-runtime-4.1.11-centos7-x86_64.rpm && \
    yum install -y ftp://ftp.renci.org/pub/irods/releases/4.1.11/centos7/irods-dev-4.1.11-centos7-x86_64.rpm && \
    rpm -i --replacefiles ftp://ftp.renci.org/pub/irods/releases/4.1.11/centos7/irods-icommands-4.1.11-centos7-x86_64.rpm

#    pip install python-irodsclient
# --upgrade pip && \
#    pip install  flask flask_restful pytest



################################################################################
#
# irods setup preparation
#
################################################################################
ADD centos7_4_1_x/app/setup_answers.txt /app/setup_answers.txt
ADD centos7_4_1_x/app/setup_irods.sh /app/setup_irods.sh
ADD app/irods_environment.json /root/.irods/irods_environment.json
ADD app/sleep.sh /app/sleep.sh
ADD app/wait_for_pg.sh /app/wait_for_pg.sh

RUN cd /app && \
    wget https://github.com/irods/irods/archive/4.1.11.tar.gz && \
    tar -xvf 4.1.11.tar.gz
    
CMD ["/app/setup_irods.sh"]
