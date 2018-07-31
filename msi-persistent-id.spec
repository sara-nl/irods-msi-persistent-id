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
install -m 755	%{libs_dir}/libmsi_pid_create.so %{buildroot}/%{irods_msi_path}/libmsi_pid_create.so
install -m 755	%{libs_dir}/libmsi_pid_delete.so %{buildroot}/%{irods_msi_path}/libmsi_pid_delete.so
install -m 755	%{libs_dir}/libmsi_pid_lookup.so %{buildroot}/%{irods_msi_path}/libmsi_pid_lookup.so
install -m 755	%{libs_dir}/libmsi_pid_update.so %{buildroot}/%{irods_msi_path}/libmsi_pid_update.so
install -m 755	%{libs_dir}/libmsi_pid_get.so    %{buildroot}/%{irods_msi_path}/libmsi_pid_get.so
install -m 755  irods_pid.json.template %{buildroot}/etc/irods/irods_pid.json.template

%files
%{irods_msi_path}/libmsi_pid_create.so
%{irods_msi_path}/libmsi_pid_delete.so
%{irods_msi_path}/libmsi_pid_lookup.so
%{irods_msi_path}/libmsi_pid_update.so
%{irods_msi_path}/libmsi_pid_get.so
/etc/irods/irods_pid.json.template

%changelog
* Tue May 31 2016 Stefan Wolfsheimer <stefan.wolfsheimer@surfsara.nl> - 1.0-1
- Todo
