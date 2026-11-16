%define oname   cars-sports-racing

%define rev     910

Name:           %{oname}-data-season2
Version:        2.1.0
Release:        %mkrel 1
Summary:        Season 2 data for Cars Sports Racing
Group:          Games/Simulation
License:        GPLv2
URL:            https://www.cars-sports-racing.net
Source0:        https://sourceforge.net/projects/cars-sports-racing/files/%{version}/%{oname}-src-season2-cars-and-tracks-%{version}-r%{rev}.tar.xz

BuildArch:      noarch
Requires:       %{oname}-data-base = %{version}
Provides:       %{oname}-data-season2 = %{version}
Obsoletes:      %{oname}-data-season2 < %{version}

%description
Season 2 data for Cars Sports Racing
- new tracks: chemisay, espie, forza,
  longpond, mile, tokyo
- new cars: 12GPA, Supercars type B
- new robots: axiom_12GPA, shadow_12GPA, usr_12GPA, axiom_scb,
  shadow_scb, usr_scb
- adds mods: 12GPA races, SC-B races

%prep
%autosetup -c

%build

%install
mkdir -p %{buildroot}%{_gamesdatadir}/%{oname}
cp -a data/* %{buildroot}%{_gamesdatadir}/%{oname}/

find %{buildroot} -name "CMakeLists.txt" -delete

%files
%{_gamesdatadir}/%{oname}/tracks/circuit/espie/
%{_gamesdatadir}/%{oname}/tracks/road/torino/
%{_gamesdatadir}/%{oname}/tracks/speedway/longpond/
%{_gamesdatadir}/%{oname}/tracks/speedway/mile/
%{_gamesdatadir}/%{oname}/tracks/speedway/tokyo/

%{_gamesdatadir}/%{oname}/config/raceman/12GPA-practice.xml
%{_gamesdatadir}/%{oname}/config/raceman/12GPA-championship*.xml
%{_gamesdatadir}/%{oname}/config/raceman/12GPA-challenge*.xml
%{_gamesdatadir}/%{oname}/config/raceman/12GPA-endurance*.xml
%{_gamesdatadir}/%{oname}/config/raceman/SC-B-practice.xml
%{_gamesdatadir}/%{oname}/config/raceman/SC-B-championship*.xml
%{_gamesdatadir}/%{oname}/config/raceman/SC-B-challenge*.xml
%{_gamesdatadir}/%{oname}/config/raceman/SC-B-endurance*.xml

%{_gamesdatadir}/%{oname}/cars/models/12gpa-deckard/
%{_gamesdatadir}/%{oname}/cars/models/12gpa-murasama/
%{_gamesdatadir}/%{oname}/cars/models/12gpa-spirit/
%{_gamesdatadir}/%{oname}/cars/models/sc-boxer-96/
%{_gamesdatadir}/%{oname}/cars/models/sc-cavallo-360/
%{_gamesdatadir}/%{oname}/cars/models/sc-fmc-gt4/
%{_gamesdatadir}/%{oname}/drivers/*_12GPA/
%{_gamesdatadir}/%{oname}/drivers/*_scb/


%changelog
* Mon Jan 19 2026 shadow72 <bertauxx@gmail.com> 2.1.0-1.mga10
- Update to Release 2.1.0
* Sat Aug 10 2024 shadow72 <bertauxx@gmail.com> 1.0.4-1.mga9
+ Revision: 1949149
- Update to release 1.0.4
* Wed May 1 2024 shadow72 <bertauxx@gmail.com> 1.0.3-1.mga9
+ Revision: 1949149
- Update to release 1.0.3
