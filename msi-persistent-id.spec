Name:           msi-persistent-id%{?branch:-%{branch}}
Version:        1.0
Release:        1%{?dist}
Summary:        Hello World example implemented in C

License:        GPLv3+
URL:            https://www.example.com/%{name}
Source0:        https://www.example.com/%{name}/releases/%{name}-%{version}.tar.gz

BuildRequires:  gcc
BuildRequires:  make

%define irods_msi_path /usr/lib/irods/plugins/microservices/
%define irods_config_path /etc/irods
%define libs_dir lib/_4.1.11
%define debug_package %{nil}

%description
Creating and managing persistent identifiers (handle) with iRods microservices.

%prep
%setup -q


%build
make %{?_smp_mflags}

%install
mkdir -p %{buildroot}/%{irods_msi_path}
install -m 755	%{libs_dir}/libmsi_pid_create.so %{buildroot}/%{irods_msi_path}/libmsi_pid_create.so
install -m 755	%{libs_dir}/libmsi_pid_delete.so %{buildroot}/%{irods_msi_path}/libmsi_pid_delete.so
install -m 755	%{libs_dir}/libmsi_pid_lookup.so %{buildroot}/%{irods_msi_path}/libmsi_pid_lookup.so
install -m 755	%{libs_dir}/libmsi_pid_update.so %{buildroot}/%{irods_msi_path}/libmsi_pid_update.so
install -m 755	%{libs_dir}/libmsi_pid_get.so    %{buildroot}/%{irods_msi_path}/libmsi_pid_get.so


%files
%{irods_msi_path}/libmsi_pid_create.so
%{irods_msi_path}/libmsi_pid_delete.so
%{irods_msi_path}/libmsi_pid_lookup.so
%{irods_msi_path}/libmsi_pid_update.so
%{irods_msi_path}/libmsi_pid_get.so

%changelog
* Tue May 31 2016 Stefan Wolfsheimer <stefan.wolfsheimer@surfsara.nl> - 1.0-1
- Todo
