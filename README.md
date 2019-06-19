# Geoloka - Géolocalisation sur architecture 8bits (ATmega328)

## Matériel utilisé
- 4 boutons [ _btn1_ - _btn4_ ]
- 1 écran LCD (8x2)
- 1 lecteur de carte SD
- 1 module GPS EM-406A
- 1 Arduino Pro Mini

## Interface et affichage

**Interface de commande**:
- _btn1_: Changer de mode d'affichage
- _btn2_: Ecrire de la donnée dans `gpslog.csv`
- _btn3_: Lire les données stockées dans `gpslog.csv`
- _btn4_: Supprimer le fichier `gpslog.csv`

**Données stockées dans** `gpslog.csv`:
- Date
- Heure
- Latitude
- Longitude
- Altitude
- Nombre de satellites
- HDOP

**Modes d'affichages**

Les modes d'affichages sont définit dans le fichier `lcd.h` par l'énumération suivante:

`enum mode { batterie, date, dt, dx, vmoy, wmode, coordonnees, altitude, hdop, nbsat};`

Détails:
- _batterie_: Affiche la valeur de tension ainsi que l'autonomie restante (en heures)
- _date_: Affiche la date et l'heure
- _dt_: Affiche le temps de parcours actuel
- _dx_: Affiche la distance parcourue actuelle
- _vmoy_: Affiche la vitesse moyenne
- _wmode_: Permet de changer de mode d'acquisition (automatique/manuel)
- _coordonnees_: Affiche les coordonées GPS
- _altitude_: Affiche l'altitude par rapport à la valeur de référence de la géodésie WGS84
- _hdop_: Affiche le HDOP
- _nbsat_: Affiche le nombre de satellites captés par le module GPS

## Documentation

  **src/**
- `btn.cpp` - Gère l'acquisition du bouton appuyé, avec filtrage des rebonds.
- `gps.cpp` - Gère l'initialisation du gps et parse le flux de donnée reçu pour l'envoyer du module GPS à l'Arduino.
- `lcd.cpp` - Gère l'affichage des différents menus, en fonction de la valeur de la variable énumération `current_mode` de type `mode`.
- `main.cpp` - Gère les initialisations au démarrage et contient la boucle principale du programme. C'est dans ce fichier que se trouve les fonctions de calcul d'autonomie et de vitesse moyenne.
- `memory.cpp` - Gère tout ce qui est relatif à la mémoire SD et EEPROM (lecture/écriture et suppression).

**include/**
- `btn.h` - Définit les valeurs des broches sur lesquelles sont câblés les boutons.
- `global.h` - Contenant des constantes ainsi que les instanciations des objets et variables _extern_ pour qu'elles soient accessibles depuis tout les fichiers du projet.
- `gps.h` - Définit les valeurs des broches TX et RX sur lesquelles est cablé le module GPS, ainsi que la vitesse de transfert.
- `lcd.h` - Définit les valeurs des broches sur lesquelles est câblé l'écran LCD, ainsi que le nombre de lignes et de colonnes de celui-ci.
Ce fichier contient également les chaines de caractères constantes destinés aux différents menus. Ces chaines sont stockées en mémoire flash pour limiter
l'utilisation de la mémoire SRAM.
- `memory.h` - Définit les valeurs des broches du lecteur de carte SD, le nom du fichier où sont stockés les données en .csv ainsi que le header csv,
c'est également dans ce fichier qu'est définit les adresses mémoires de l'EEPROM que l'on utilise pour stocker des variables.




**TODO**
- Affichage pour contenu de la carte SD + place disponible
- Améliorer la navigation du menu
- Faire script pour conversion .csv to .gpx
- Résoudre le problème de display(mode m) en switch statements
