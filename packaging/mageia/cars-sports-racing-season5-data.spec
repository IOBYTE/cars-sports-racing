%define oname   cars-sports-racing

%define rev     645

Name:           %{oname}-data-season5
Version:        1.0.6
Release:        %mkrel 1
Summary:        Season 5 data for Cars Sports Racing
Group:          Games/Simulation
License:        GPLv2
URL:            https://www.cars-sports-racing.net
Source0:        https://sourceforge.net/projects/cars-sports-racing/files/%{version}/%{oname}-src-season5-cars-and-tracks-%{version}.tar.xz

BuildArch:      noarch
Requires:       %{oname}-data-base = %{version}
Requires:       %{oname}-data-season2 = %{version}
Requires:       %{oname}-data-season3 = %{version}
Requires:       %{oname}-data-season4 = %{version}
Provides:       %{oname}-data-season5 = %{version}

%description
Season 5 data for Cars Sports Racing
- new tracks: dijon, jarama, hidden-valley, migrants, ruudskogen
  brighton, cagliari, casablanca, garguree, ole-road-1
  atlanta, longday, salty
- new cars: LS1, Hypercars
- new robots: axiom_ls1, shadow_ls1, usr_ls1, axiom_hc,
  shadow_hc, usr_hc
- adds mods: LS1 races, Hypercars races

%prep
%autosetup -c

%build

%install
mkdir -p %{buildroot}%{_gamesdatadir}/%{oname}
cp -a data/* %{buildroot}%{_gamesdatadir}/%{oname}/

find %{buildroot} -name "CMakeLists.txt" -delete

%files
%{_gamesdatadir}/%{oname}/tracks/circuit/dijon/
%{_gamesdatadir}/%{oname}/tracks/circuit/jarama/
%{_gamesdatadir}/%{oname}/tracks/circuit/hidden-valley/
%{_gamesdatadir}/%{oname}/tracks/circuit/migrants/
%{_gamesdatadir}/%{oname}/tracks/circuit/ruudskogen/

%{_gamesdatadir}/%{oname}/tracks/dirt/brighton
%{_gamesdatadir}/%{oname}/tracks/dirt/cagliari
%{_gamesdatadir}/%{oname}/tracks/dirt/casablanca
%{_gamesdatadir}/%{oname}/tracks/dirt/garguree
%{_gamesdatadir}/%{oname}/tracks/dirt/ole-road-1

%{_gamesdatadir}/%{oname}/tracks/road/atlanta/
%{_gamesdatadir}/%{oname}/tracks/road/longday/
%{_gamesdatadir}/%{oname}/tracks/road/salty/

%{_gamesdatadir}/%{oname}/config/raceman/LS1-practice.xml
%{_gamesdatadir}/%{oname}/config/raceman/LS1-championship*.xml
%{_gamesdatadir}/%{oname}/config/raceman/LS1-challenge*.xml
%{_gamesdatadir}/%{oname}/config/raceman/LS1-endurance*.xml

%{_gamesdatadir}/%{oname}/config/raceman/HC-practice.xml
%{_gamesdatadir}/%{oname}/config/raceman/HC-championship*.xml
%{_gamesdatadir}/%{oname}/config/raceman/HC-challenge*.xml
%{_gamesdatadir}/%{oname}/config/raceman/HC-endurance*.xml

%{_gamesdatadir}/%{oname}/cars/models/ls2-bavaria-g3gtr/
%{_gamesdatadir}/%{oname}/cars/models/ls2-bavaria-nb/
%{_gamesdatadir}/%{oname}/cars/models/ls2-boxer-96rsr/
%{_gamesdatadir}/%{oname}/cars/models/ls2-cavallo-360r/
%{_gamesdatadir}/%{oname}/cars/models/ls2-condor-c400r/
%{_gamesdatadir}/%{oname}/cars/models/ls2-lynx-xkr/
%{_gamesdatadir}/%{oname}/cars/models/ls2-morley-r900/
%{_gamesdatadir}/%{oname}/cars/models/ls2-sprite-gts/
%{_gamesdatadir}/%{oname}/cars/models/ls2-vortex-gt/
%{_gamesdatadir}/%{oname}/cars/models/ls2-vulture-v6rs2/

%{_gamesdatadir}/%{oname}/cars/models/rs1-citrus-cs4/
%{_gamesdatadir}/%{oname}/cars/models/rs1-fmc-centre/
%{_gamesdatadir}/%{oname}/cars/models/rs1-kenji-08/
%{_gamesdatadir}/%{oname}/cars/models/rs2-fai-dot/
%{_gamesdatadir}/%{oname}/cars/models/rs2-lion-2m/

%{_gamesdatadir}/%{oname}/drivers/*_ls1/
%{_gamesdatadir}/%{oname}/drivers/*_hc/

%changelog
* Sat Aug 10 2024 shadow72 <bertauxx@gmail.com> 1.0.5-1.mga9
+ Revision: 1949149
- New season to release 1.0.5
