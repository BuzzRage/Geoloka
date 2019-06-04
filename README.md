# Geoloka

## Géolocalisation sur architecture 8bits (ATmega328)


**I/O**:
- 4 boutons [ _btn1_ - _btn4_ ]
- 1 écran LCD (8x2)
- 1 carte SD
- 1 module GPS

**Interface de commande**:
- _btn1_: Changer de mode d'affichage
- _btn2_: Ecrire de la donnée dans `datalog.txt`
- _btn3_: Lire de la donnée stockée dans `datalog.txt`
- _btn4_: Supprimer le fichier `datalog.txt`

**Données stockées dans** `datalog.txt`:
- Temps écoulé depuis le démarrage (en ms)
- Valeur de tension (en V)

**TODO**
- Affichage pour contenu de la carte SD + place disponible
- Faire relevés de points pendant 1h, puis calculer la moyenne/écart-type/DOP:
  - Dans un champ dégagé
  - Dans un milieu boisé
  - A l'UTT
- Faire un relevé de points au cours d'un trajet bien défini
- Calculer l'autonomie de la batterie
