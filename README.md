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
Les modes d'affichages sont définit dans le fichier _lcd.h_ par l'énumération suivante:
_enum mode { batterie, date, dt, dx, vmoy, wmode, coordonnees, altitude, hdop, nbsat};_

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

##Documentation
  **src/**
    - _btn.cpp_ - Gère l'acquisition du bouton appuyé, avec filtrage des rebonds.
    - _gps.cpp_ - Gère l'initialisation du gps et parse le flux de donnée reçu pour l'envoyer du module GPS à l'Arduino.
    - _lcd.cpp_ - Gère l'affichage des différents menus, en fonction de la valeur de la variable énumération _current_mode_ de type _mode_.
    - _main.cpp_ - Gère les initialisations au démarrage et contient la boucle principale du programme. C'est dans ce fichier que se trouve les fonctions de calcul d'autonomie et de vitesse moyenne.
    - _memory.cpp_ - Gère tout ce qui est relatif à la mémoire SD et EEPROM (lecture/écriture et suppression).

**include/**
    - _btn.h_ - Définit les valeurs des broches sur lesquelles sont câblés les boutons.
    - _global.h_ - Contenant des constantes ainsi que les instanciations des objets et variables _extern_ pour qu'elles soient accessibles depuis tout les fichiers du projet.
    - _gps.h_ - Définit les valeurs des broches TX et RX sur lesquelles est cablé le module GPS, ainsi que la vitesse de transfert.
    - _lcd.h_ - Définit les valeurs des broches sur lesquelles est câblé l'écran LCD, ainsi que le nombre de lignes et de colonnes de celui-ci.
    Ce fichier contient également les chaines de caractères constantes destinés aux différents menus. Ces chaines sont stockées en mémoire flash pour limiter
    l'utilisation de la mémoire SRAM.
    - _memory.h_ - Définit les valeurs des broches du lecteur de carte SD, le nom du fichier où sont stockés les données en .csv ainsi que le header csv,
    c'est également dans ce fichier qu'est définit les adresses mémoires de l'EEPROM que l'on utilise pour stocker des variables.



**TODO**
- Affichage pour contenu de la carte SD + place disponible
- Améliorer la navigation du menu
- Faire script pour conversion .csv to .gpx
- Résoudre le problème de display(mode m) en switch statements
