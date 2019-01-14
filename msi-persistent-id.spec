Name:           %{packagename}
Version:        %{version}
Release:        1%{?dist}
Summary:        Integration of iRODS and handle system via microservices.

License:        GPLv3+
Source0:        %{packagename}-%{version}.tar.gz

%if "%{irodsversion}" == "4.1.11"
%define irods_msi_path /var/lib/irods/plugins/microservices
%else
%define irods_msi_path /usr/lib/irods/plugins/microservices
%endif

%define libs_dir lib/_%{irodsversion}
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
install -m 755	%{libs_dir}/libmsiPidDeleteHandle.so %{buildroot}/%{irods_msi_path}/libmsiPidDeleteHandle.so
install -m 755	%{libs_dir}/libmsiPidLookup.so %{buildroot}/%{irods_msi_path}/libmsiPidLookup.so
install -m 755	%{libs_dir}/libmsiPidLookupOne.so %{buildroot}/%{irods_msi_path}/libmsiPidLookupOne.so
install -m 755	%{libs_dir}/libmsiPidLookupKey.so %{buildroot}/%{irods_msi_path}/libmsiPidLookupKey.so
install -m 755	%{libs_dir}/libmsiPidMove.so %{buildroot}/%{irods_msi_path}/libmsiPidMove.so
install -m 755	%{libs_dir}/libmsiPidGet.so %{buildroot}/%{irods_msi_path}/libmsiPidGet.so
install -m 755	%{libs_dir}/libmsiPidSet.so %{buildroot}/%{irods_msi_path}/libmsiPidSet.so
install -m 755	%{libs_dir}/libmsiPidUnset.so %{buildroot}/%{irods_msi_path}/libmsiPidUnset.so
install -m 755  irods_pid.json.template %{buildroot}/etc/irods/irods_pid.json.template
install -m 755  irods_pid.json.template_2 %{buildroot}/etc/irods/irods_pid.json.template_2



%files
%{irods_msi_path}/libmsiPidCreate.so
%{irods_msi_path}/libmsiPidDelete.so
%{irods_msi_path}/libmsiPidDeleteHandle.so
%{irods_msi_path}/libmsiPidLookup.so
%{irods_msi_path}/libmsiPidLookupOne.so
%{irods_msi_path}/libmsiPidLookupKey.so
%{irods_msi_path}/libmsiPidMove.so
%{irods_msi_path}/libmsiPidGet.so
%{irods_msi_path}/libmsiPidSet.so
%{irods_msi_path}/libmsiPidUnset.so
/etc/irods/irods_pid.json.template
/etc/irods/irods_pid.json.template_2

%post
if [ -e /etc/irods/service_account.config ]
then
    source /etc/irods/service_account.config
    chown $IRODS_SERVICE_ACCOUNT_NAME:$IRODS_SERVICE_GROUP_NAME %{irods_msi_path}/libmsiPidCreate.so
    chmod 755 %{irods_msi_path}/libmsiPidCreate.so

    chown $IRODS_SERVICE_ACCOUNT_NAME:$IRODS_SERVICE_GROUP_NAME %{irods_msi_path}/libmsiPidDelete.so
    chmod 755 %{irods_msi_path}/libmsiPidDelete.so

    chown $IRODS_SERVICE_ACCOUNT_NAME:$IRODS_SERVICE_GROUP_NAME %{irods_msi_path}/libmsiPidDeleteHandle.so
    chmod 755 %{irods_msi_path}/libmsiPidDeleteHandle.so

    chown $IRODS_SERVICE_ACCOUNT_NAME:$IRODS_SERVICE_GROUP_NAME %{irods_msi_path}/libmsiPidLookup.so
    chmod 755 %{irods_msi_path}/libmsiPidLookup.so

    chown $IRODS_SERVICE_ACCOUNT_NAME:$IRODS_SERVICE_GROUP_NAME %{irods_msi_path}/libmsiPidLookupOne.so
    chmod 755 %{irods_msi_path}/libmsiPidLookupOne.so

    chown $IRODS_SERVICE_ACCOUNT_NAME:$IRODS_SERVICE_GROUP_NAME %{irods_msi_path}/libmsiPidLookupKey.so
    chmod 755 %{irods_msi_path}/libmsiPidLookupKey.so

    chown $IRODS_SERVICE_ACCOUNT_NAME:$IRODS_SERVICE_GROUP_NAME %{irods_msi_path}/libmsiPidMove.so
    chmod 755 %{irods_msi_path}/libmsiPidMove.so

    chown $IRODS_SERVICE_ACCOUNT_NAME:$IRODS_SERVICE_GROUP_NAME %{irods_msi_path}/libmsiPidGet.so
    chmod 755 %{irods_msi_path}/libmsiPidGet.so

    chown $IRODS_SERVICE_ACCOUNT_NAME:$IRODS_SERVICE_GROUP_NAME %{irods_msi_path}/libmsiPidSet.so
    chmod 755 %{irods_msi_path}/libmsiPidSet.so

    chown $IRODS_SERVICE_ACCOUNT_NAME:$IRODS_SERVICE_GROUP_NAME %{irods_msi_path}/libmsiPidUnset.so
    chmod 755 %{irods_msi_path}/libmsiPidUnset.so

    chown $IRODS_SERVICE_ACCOUNT_NAME:$IRODS_SERVICE_GROUP_NAME /etc/irods/irods_pid.json.template
    chown $IRODS_SERVICE_ACCOUNT_NAME:$IRODS_SERVICE_GROUP_NAME /etc/irods/irods_pid.json.template_2
    chmod 644 /etc/irods/irods_pid.json.template
    chmod 644 /etc/irods/irods_pid.json.template_2
fi

%changelog
* Mon Dec 10 2018 Stefan Wolfsheimer <stefan.wolfsheimer@surfsara.nl> - develop
- template mechanism for dynamic handle profiles

* Tue Nov 20 2018 Stefan Wolfsheimer <stefan.wolfsheimer@surfsara.nl> - develop
- add lookup by key

* Tue Nov 13 2018 Stefan Wolfsheimer <stefan.wolfsheimer@surfsara.nl> - develop
- add support for iRODS 4.2.3

* Wed Aug 01 2018 Stefan Wolfsheimer <stefan.wolfsheimer@surfsara.nl> - develop
- initial development branch
