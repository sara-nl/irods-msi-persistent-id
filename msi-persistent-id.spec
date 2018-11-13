Name:           %{packagename}
Version:        %{version}
Release:        1%{?dist}
Summary:        Integration of iRODS and handle system via microservices.

License:        GPLv3+
Source0:        %{packagename}-%{version}.tar.gz

%if "%{irodsversion}" == "4_2_3"
%define irods_msi_path /usr/lib/irods/plugins/microservices/
%define libs_dir lib/_4.2.3
%else
%define irods_msi_path /var/lib/irods/plugins/microservices/
%define libs_dir lib/_4.1.11
%endif

%define irods_config_path /etc/irods
%define debug_package %{nil}

%description
Creating and managing persistent identifiers (handle) with iRods microservices.

%prep
%setup -q


%build
make

%install
mkdir -p %{buildroot}/%{irods_msi_path}
mkdir -p %{buildroot}/etc/irods
install -m 755	%{libs_dir}/libmsiPidCreate.so %{buildroot}/%{irods_msi_path}/libmsiPidCreate.so
install -m 755	%{libs_dir}/libmsiPidDelete.so %{buildroot}/%{irods_msi_path}/libmsiPidDelete.so
install -m 755	%{libs_dir}/libmsiPidLookup.so %{buildroot}/%{irods_msi_path}/libmsiPidLookup.so
install -m 755	%{libs_dir}/libmsiPidMove.so %{buildroot}/%{irods_msi_path}/libmsiPidMove.so
install -m 755	%{libs_dir}/libmsiPidGet.so %{buildroot}/%{irods_msi_path}/libmsiPidGet.so
install -m 755	%{libs_dir}/libmsiPidSet.so %{buildroot}/%{irods_msi_path}/libmsiPidSet.so
install -m 755	%{libs_dir}/libmsiPidUnset.so %{buildroot}/%{irods_msi_path}/libmsiPidUnset.so
install -m 755  irods_pid.json.template %{buildroot}/etc/irods/irods_pid.json.template

%files
%{irods_msi_path}/libmsiPidCreate.so
%{irods_msi_path}/libmsiPidDelete.so
%{irods_msi_path}/libmsiPidLookup.so
%{irods_msi_path}/libmsiPidMove.so
%{irods_msi_path}/libmsiPidGet.so
%{irods_msi_path}/libmsiPidSet.so
%{irods_msi_path}/libmsiPidUnset.so
/etc/irods/irods_pid.json.template

%post
if [ -e /etc/irods/service_account.config ]
then
    source /etc/irods/service_account.config
    chown $IRODS_SERVICE_ACCOUNT_NAME:$IRODS_SERVICE_GROUP_NAME %{irods_msi_path}/libmsiPidCreate.so
    chmod 644 %{irods_msi_path}/libmsiPidCreate.so
    chown $IRODS_SERVICE_ACCOUNT_NAME:$IRODS_SERVICE_GROUP_NAME %{irods_msi_path}/libmsiPidDelete.so
    chmod 644 %{irods_msi_path}/libmsiPidDelete.so
    chown $IRODS_SERVICE_ACCOUNT_NAME:$IRODS_SERVICE_GROUP_NAME %{irods_msi_path}/libmsiPidLookup.so
    chmod 644 %{irods_msi_path}/libmsiPidLookup.so
    chown $IRODS_SERVICE_ACCOUNT_NAME:$IRODS_SERVICE_GROUP_NAME %{irods_msi_path}/libmsiPidMove.so
    chmod 644 %{irods_msi_path}/libmsiPidMove.so
    chown $IRODS_SERVICE_ACCOUNT_NAME:$IRODS_SERVICE_GROUP_NAME %{irods_msi_path}/libmsiPidGet.so
    chmod 644 %{irods_msi_path}/libmsiPidGet.so
    chown $IRODS_SERVICE_ACCOUNT_NAME:$IRODS_SERVICE_GROUP_NAME %{irods_msi_path}/libmsiPidSet.so
    chmod 644 %{irods_msi_path}/libmsiPidSet.so
    chown $IRODS_SERVICE_ACCOUNT_NAME:$IRODS_SERVICE_GROUP_NAME %{irods_msi_path}/libmsiPidUnset.so
    chmod 644 %{irods_msi_path}/libmsiPidUnset.so
    chown $IRODS_SERVICE_ACCOUNT_NAME:$IRODS_SERVICE_GROUP_NAME /etc/irods/irods_pid.json.template
    chmod 644 /etc/irods/irods_pid.json.template
fi

%changelog
* Tue Nov 13 2018 Stefan Wolfsheimer <stefan.wolfsheimer@surfsara.nl> - develop
- add support for iRODS 4.2.3

* Wed Aug 01 2018 Stefan Wolfsheimer <stefan.wolfsheimer@surfsara.nl> - develop
- initial development branch
