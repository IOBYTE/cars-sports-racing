%define         _disable_ld_no_undefined 1

%define         rev     910
Name:           cars-sports-racing
Version:        2.1.0
Release:        %mkrel 1
Summary:        Cars Sports Racing: Simulation
Group:          Games/Simulation
License:        GPLv2
URL:            https://www.cars-sports-racing.net
Source0:        https://sourceforge.net/projects/cars-sports-racing/files/%{version}/%{name}-src-base-%{version}-r%{rev}.tar.xz

BuildRequires:  cmake
BuildRequires:  pkgconfig(libjpeg)
BuildRequires:  pkgconfig(expat)
BuildRequires:  pkgconfig(freealut)
BuildRequires:  pkgconfig(freeglut)
BuildRequires:  pkgconfig(gl)
BuildRequires:  pkgconfig(libcurl)
BuildRequires:  pkgconfig(libenet)
BuildRequires:  pkgconfig(libpng)
BuildRequires:  pkgconfig(openal)
BuildRequires:	pkgconfig(openscenegraph)
BuildRequires:  pkgconfig(sdl2)
BuildRequires:  pkgconfig(SDL2_mixer)
BuildRequires:  pkgconfig(vorbis)
BuildRequires:  pkgconfig(zlib)
BuildRequires:  mesa-common-devel
#(eatdirt) let's move to 3.6
BuildRequires:  plib-devel
BuildRequires:  chrpath
BuildRequires:  java-devel

Requires:       %{name}-data-base = %{version}
Obsoletes:		%{name} < %{version}

%description
A fork of the open racing car simulator Speed Dreams, itself being a fork of Torcs
This project implementing challenges for the player to pass allowing them to unlock the levels
For the moment, just OpenGL1 (with PLIB) is implemented.
Best lap recorded in masterserver and in the future, hall of fame for championship ranking
The goal in the future is to have a career mode integrated into the game engine

%package        data-base
Summary:        Base data for Cars Sports Racing
BuildArch:      noarch
Provides:       %{name}-data-base = %{version}
Obsoletes:      %{name}-data-base < %{version}


%description    data-base
Base data for Cars Sports Racing. 
Contains the tracks "alicante", "auriac", "alamo", "arizona", "corn", "manton", "newton",
"brondehach", "karwada", "peters".
Contains the cars "09gp5-fmc", "sc-deckard-conejo", "sc-kanagawa-z35gts", "sc-murasama-nsx"

%prep
%setup -q -c
%autopatch -p1

# fix unmappable characters in comments
for i in ./src/tools/trackeditor/gui/view/CircuitView.java \
	./src/tools/trackeditor/utils/circuit/ObjShapeHandle.java \
	./src/tools/trackeditor/utils/circuit/ObjShapeTerrain.java \
	./src/tools/trackeditor/utils/circuit/XmlObjPits.java; do
	cat ${i} | iconv -c -t latin1 > ${i}.new
	mv -f ${i}.new ${i}
done

%build
# Review options in cmake/options.cmake on new releases

%cmake \
	-DCMAKE_POLICY_VERSION_MINIMUM=3.5 \
    -DBUILD_SHARED_LIBS=OFF \
	-DCMAKE_SKIP_INSTALL_RPATH=OFF \
	-DCMAKE_SKIP_RPATH=OFF \ \
    -DOPTION_3RDPARTY_EXPAT=ON \
    -DOPTION_3RDPARTY_SOLID=OFF \
    -DOPTION_AUTOVERSION=OFF \
    -DOPTION_OFFICIAL_ONLY=ON \
	-DOPTION_PACKAGING=ON \
	-DOPTION_CLIENT_SERVER=ON \
	-DOPTION_WEBSERVER=ON \
	-DOPTION_OPENGL3=ON \
	-D CSR_LOCALDIR=~/.config/cars-sports-racing   
%cmake_build

%install
%cmake_install

rm -rf %{buildroot}%{_gamesdatadir}/%{name}/cmake
rm -rf %{buildroot}%{_gamesdatadir}/%{name}/*.txt
rm -rf %{buildroot}%{_includedir}

mkdir -p %{buildroot}%{_datadir}/applications
cat > %{buildroot}%{_datadir}/applications/%{name}.desktop << EOF
[Desktop Entry]
Name=Cars Sports Racing
Comment=Simulation
Exec=%{name}
Icon=%{name}
Terminal=false
Type=Application
StartupNotify=false
Categories=Game;SportsGame;
EOF

install -D -m644 data/data/icons/icon.png %{buildroot}%{_iconsdir}/%{name}.png

mkdir -p %{buildroot}%{_mandir}/man6
install -m644 doc/man/* %{buildroot}%{_mandir}/man6/

#(eatdirt) cannot build and run without RPATH, let's nuke the invalid ones now
pushd %{buildroot}%{_libdir}/games/%{name}/drivers
for mod in $(find . -type f -name \*.so ); do
    chrpath --delete $mod
done
popd

%files
%doc CHANGES.txt COPYING.txt README.txt MANUAL.txt MANUAL-FR.txt ROADMAP.txt
%{_datadir}/applications/%{name}.desktop
%{_gamesbindir}/csr-*
%{_gamesbindir}/%{name}
%{_gamesbindir}/bsh-2.0b4.jar
%{_gamesbindir}/jdom-1.1.3.jar
%{_gamesbindir}/jgoodies-common-1.8.1.jar
%{_gamesbindir}/jgoodies-looks-2.5.3.jar

%{_iconsdir}/%{name}.png
%dir %{_libdir}/games/%{name}
%{_libdir}/games/%{name}/drivers/
%{_libdir}/games/%{name}/lib/
%{_libdir}/games/%{name}/modules/
%{_mandir}/man6/csr-*.6*
%{_mandir}/man6/%{name}.6*

%files data-base
%dir %{_gamesdatadir}/%{name}
%{_gamesdatadir}/%{name}/*

%changelog
* Mon Jan 19 2026 shadow72 <bertauxx@gmail.com> 2.1.0-1.mga10
- Update to Release 2.1.0
* Fri Jun 06 2025 shadow72 <bertauxx@gmail.com> 2.0.0-1.mga9
- Update to release 2.0.0
* Sat Aug 10 2024 shadow72 <bertauxx@gmail.com> 1.0.4-1.mga9
+ Revision: 1949149
- Update to release 1.0.4
* Wed May 1 2024 shadow72 <bertauxx@gmail.com> 1.0.3-1.mga9
+ Revision: 1949149
- Update to release 1.0.3
