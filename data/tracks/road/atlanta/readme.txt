Track Atlanta - Race road in town

Generate raceline: ~/torcs_bin/bin/trackgen -c road -n atlanta -r
Generate shading: ~/torcs_bin/bin/accc +shad atlanta-shade-src.ac atlanta-shade.ac (shading file was manually edited)
Generate acc: ~/torcs_bin/bin/accc -g atlanta.acc -l0 atlanta-src.ac -l1 atlanta-shade.ac -l2 atlanta-trk-raceline.ac -d3 1000 -d2 500 -d1 300 -S 300 -es

--
Copyright © 2022 Xavier Bertaux

Copyleft: this work of art is free, you can redistribute
it and/or modify it according to terms of the Free Art license.
You will find a specimen of this license on the site
Copyleft Attitude http://artlibre.org as well as on other sites.
