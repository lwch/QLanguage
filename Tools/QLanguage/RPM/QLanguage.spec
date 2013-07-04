Name:		QLanguage
Version:	1.0
Release:	1%{?dist}
Summary:	QLanguage Compiler

Group:		Development/Tools
License:	GPL
URL:		http://code.google.com/p/qlanguage
Source:		QLanguage.tar.gz
BuildRoot:	%{_tmppath}/%{name}-%{version}-%{release}-root

BuildRequires:	gcc-c++ cmake make

%description
QLanguage Compiler

%prep
%setup -q -n %{name}

%build
cd Source/
cmake -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
make


%install
rm -rf %{buildroot}
cd Source/
make install DESTDIR=%{buildroot}


%clean
rm -rf %{buildroot}


%files
%defattr(-,root,root,-)
/usr/local/bin/QLanguage
/usr/local/bin/QLanguage.os.ParserTable

%changelog

