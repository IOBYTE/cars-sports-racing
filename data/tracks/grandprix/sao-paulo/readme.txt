1. Generate raceline: ~/torcs_bin/bin/trackgen -c road -n sao-paulo -r
2. Edit raceline to match hierarchy in sao-paulo-src.ac (TKMN0, TKMN1)
3. Generate shading: ~/torcs_bin/bin/accc +shad sao-paulo-src.ac sao-paulo-shade-src.ac
4. Edit sao-paulo-shade-src.ac to use shadow.png
5. Generate acc: ~/torcs_bin/bin/accc -g sao-paulo.acc -l0 sao-paulo-src.ac -l1 sao-paulo-shade-src.ac -l2 sao-paulo-trk-raceline-src.ac -d3 1000 -d2 500 -d1 300 -S 300 -es

Track "Sao Paulo"
------------------------------------------------------------------------------------
Copyright © 2011
Remake 2020 by Xavier Bertaux

Copyleft: this work of art is free, you can redistribute
it and/or modify it according to terms of the Free Art license.
You will find a specimen of this license on the site
Copyleft Attitude http://artlibre.org as well as on other sites.

------------------------------------------------------------------------------------
