Name:           %{packagename}
Version:        %{version}
Release:        1%{?dist}
Summary:        Integration of iRODS and handle system via microservices.

License:        GPLv3+
Source0:        %{packagename}-%{version}.tar.gz

BuildRequires:  gcc
BuildRequires:  make

%define irods_msi_path /var/lib/irods/plugins/microservices/
%define irods_config_path /etc/irods
%define libs_dir lib/_4.1.11
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
install -m 755	%{libs_dir}/libmsiPidGet.so    %{buildroot}/%{irods_msi_path}/libmsiPidGet.so
install -m 755  irods_pid.json.template %{buildroot}/etc/irods/irods_pid.json.template

%files
%{irods_msi_path}/libmsiPidCreate.so
%{irods_msi_path}/libmsiPidDelete.so
%{irods_msi_path}/libmsiPidLookup.so
%{irods_msi_path}/libmsiPidMove.so
%{irods_msi_path}/libmsiPidGet.so
/etc/irods/irods_pid.json.template

%changelog
* Wed Aug 01 2018 Stefan Wolfsheimer <stefan.wolfsheimer@surfsara.nl> - develop
- initial development branch
