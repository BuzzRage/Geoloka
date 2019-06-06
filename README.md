# Geoloka

## Géolocalisation sur architecture 8bits (ATmega328)


**I/O**:
- 4 boutons [ _btn1_ - _btn4_ ]
- 1 écran LCD (8x2)
- 1 carte SD
- 1 module GPS EM-406A

**Interface de commande**:
- _btn1_: Changer de mode d'affichage
- _btn2_: Ecrire de la donnée dans `datalog.txt`
- _btn3_: Lire de la donnée stockée dans `datalog.txt`
- _btn4_: Supprimer le fichier `datalog.txt`

**Données stockées dans** `gpslog.csv`:
- Date
- Heure
- Latitude
- Longitude

**TODO**
- Affichage pour contenu de la carte SD + place disponible
- Améliorer la navigation du menu
- Faire script pour conversion .csv to .gpx
- Calculer l'autonomie de la batterie (en %age et en durée restante)
- Corriger heure (pour 1h, ça affiche 25h)
- Résoudre le problème de display(mode m) en switch statements
