%define oname   cars-sports-racing

%define rev     910

Name:           %{oname}-data-season3
Version:        2.1.0
Release:        %mkrel 1
Summary:        Season 3 data for Cars Sports Racing
Group:          Games/Simulation
License:        GPLv2
URL:            https://www.cars-sports-racing.net
Source0:        https://sourceforge.net/projects/cars-sports-racing/files/%{version}/%{oname}-src-season3-cars-and-tracks-%{version}-r%{rev}.tar.xz

BuildArch:      noarch
Requires:       %{oname}-data-base = %{version}
Requires:       %{oname}-data-season2 = %{version}
Provides:       %{oname}-data-season3 = %{version}
Obsoletes:      %{oname}-data-season3 < %{version}

%description
Season 3 data for Cars Sports Racing
- new tracks: corkscrew, prenzlau, milky-five
- new cars: 01GPA, Supercars class A
- new robots: axiom_01GPA, shadow_01GPA, usr_01GPA, axiom_sca,
  shadow_sca, usr_sca
- adds mods: 01GPA races, SC-A races

%prep
%autosetup -c

%build

%install
mkdir -p %{buildroot}%{_gamesdatadir}/%{oname}
cp -a data/* %{buildroot}%{_gamesdatadir}/%{oname}/

find %{buildroot} -name "CMakeLists.txt" -delete

%files
%{_gamesdatadir}/%{oname}/tracks/grandprix/corkscrew/
%{_gamesdatadir}/%{oname}/tracks/road/prenzlau/
%{_gamesdatadir}/%{oname}/tracks/speedway/milky-five/

%{_gamesdatadir}/%{oname}/config/raceman/01GPA-practice.xml
%{_gamesdatadir}/%{oname}/config/raceman/01GPA-championship*.xml
%{_gamesdatadir}/%{oname}/config/raceman/01GPA-challenge*.xml
%{_gamesdatadir}/%{oname}/config/raceman/01GPA-endurance*.xml
%{_gamesdatadir}/%{oname}/config/raceman/SC-A-practice.xml
%{_gamesdatadir}/%{oname}/config/raceman/SC-A-championship*.xml
%{_gamesdatadir}/%{oname}/config/raceman/SC-A-challenge*.xml
%{_gamesdatadir}/%{oname}/config/raceman/SC-A-endurance*.xml

%{_gamesdatadir}/%{oname}/cars/models/01gpa-ffr/
%{_gamesdatadir}/%{oname}/cars/models/01gpa-furia/
%{_gamesdatadir}/%{oname}/cars/models/01gpa-hartbill-2002/
%{_gamesdatadir}/%{oname}/cars/models/01gpa-murasama/
%{_gamesdatadir}/%{oname}/cars/models/01gpa-team-lynx/
%{_gamesdatadir}/%{oname}/cars/models/sc-lynx-220/
%{_gamesdatadir}/%{oname}/cars/models/sc-spirit-300/
%{_gamesdatadir}/%{oname}/cars/models/sc-vieringe-4s/
%{_gamesdatadir}/%{oname}/drivers/*_01GPA/
%{_gamesdatadir}/%{oname}/drivers/*_sca/

%changelog
* Mon Jan 19 2026 shadow72 <bertauxx@gmail.com> 2.1.0-1.mga10
- Update to Release 2.1.0
* Sat Aug 10 2024 shadow72 <bertauxx@gmail.com> 1.0.4-1.mga9
+ Revision: 1949149
- Update to release 1.0.4
* Wed May 1 2024 shadow72 <bertauxx@gmail.com> 1.0.3-1.mga9
+ Revision: 1949149
- Update to release 1.0.3
