# 2024-2025-4GP-HILLAH-JACQUET
---
## Sommaire 
   - [I) Contexte et objectifs du projet](#I-Contexteetobjectifsduprojet)
   - [II) Capteur à jauge de contrainte (Flex Sensor) à base de crayon graphite](#II-Capteuràjaugedecontrainte(FlexSensor)àbasedecrayongraphite)
   - [III) Electronique Analogique](#III-Electroniqueanalogique)
   - [IV) KiCad et PCB](#IV-KiCadetPCB)
   - [V) Arduino](#V-Arduino)
   - [VI) Application Mobile](#VI-ApplicationMobile)
   - [VII) Banc de Test](#VII-BancdeTest)
   - [VIII) Datasheet](#VIII-Datasheet)

---

## I) Contexte et objectifs du projet

L'objectif de ce projet est de réaliser un capteur low-tech à base de graphite.  Les recherches ont démontré les nombreux avantages du carbone graphite. L’électronique à base de papier gagne en popularité parmi les ingénieurs grâce à sa facilité d’approvisionnement, sa simplicité de fabrication et son faible coût.<br>
Pour ce faire, nous avons mis en place la réalisation de tous les processus de fabrication d'un shield PCB que nous avons ensuite connecté à une carte Arduino.<br>
Notre PCB contient 6 composants différents : une jauge de contrainte, un flex sensor, un écran oled, un encodeur rotatoire, un potentiomètre digital et un module Bluetooth. À noter que nous n'avons pas utilisé le servo-moteur qui faisait partie des composants disponibles à implémenter.<br>
La carte Arduino fonctionne avec un code que nous avons implémenté en s'inspirant grandement des codes qui étaient disponibles en open-source.<br>
Nous avons ensuite réalisé une application mobile connectée au module bluetooth afin d'acquérir les données renvoyées par nos capteurs sur le téléphone.<br>
Nous avons évalué le capteur sur un banc de test avant de conclure par la datasheet de notre capteur.<br>

## II)Capteur à jauge de contrainte (Flex Sensor) à base de crayon graphite

Ce capteur low-tech utilise la théorie de la percolation au sein d’un système granulaire constitué de nanoparticules de graphite.<br>
Nous déposons une fine couche de graphite sur la feuille de papier ce qui présente un réseau percolé caractéristique. Dans ce réseau, le transport des électrons entre les nanoparticules est basé sur le mécanisme de l’effet tunnel.<br>
Lors de l'application de déformations en tension, le réseau percolé se trouve directement étendu. Cette déformation entraîne une augmentation de la distance effective entre les particules de graphite au sein du réseau. Une partie des chemins de percolation sont par conséquent rompus. Ainsi, la conduction du matériau diminue, donc la résistance de la couche de graphite augmente.<br>
Inversement, lors de l’application de déformations en compression, le réseau percolé se trouve directement comprimé. Cette déformation entraîne une diminution de la distance effective entre les particules.<br> Ainsi, de nouveaux chemins de percolation sont créés, la conduction du matériau augmente, soit la résistance de la couche de graphite diminue.

Une jauge de contrainte est un circuit résistif dont la résistance varie en fonction de sa déformation. Donc en mesurant les variations de résistance de la jauge, nous en avons déduit la contrainte et la déformation appliquées.<br>
Notre jauge de contrainte est alimentée par une tension régulée de 5V d'une carte Arduino UNO.
<br>
Voici ci dessous le schéma d'un modèle de capteur graphite. Dans notre projet, nous avons utilisé les crayons de dureté 2H ; 2B ; HB.

<p align="center">
<img src=https://github.com/MOSH-Insa-Toulouse/2024-2025-4GP-divdav7-EnzoJ26/blob/main/Images_Projet/Mod%C3%A8le%20de%20capteur%20graphite.JPG alt="Figure 1 - Capteur à jauge de contrainte à base de crayon graphite">
<br>
<i>Capteur à jauge de contrainte à base de crayon graphite</i>
</p>

## III) Electronique Analogique

Ce capteur graphite possède une résistance variable de l'ordre du GΩ. Le courant généré lorsque l'on applique une tension de 5V aux bornes du capteur est très faible.<br>
Pour obtenir un signal exploitable nous avons dû l'amplifier. Pour ce faire, nous avons utilisé un montage transimpédance constitué d'un amplificateur opérationnel.<br>
Pour cela nous avons dans un premier temps effectué une simulation électronique du capteur et de ce montage transimpédance sous LTSpice. Voici ci-dessous le montage réalisé sous LTSpice. Pour notre montage nous avons choisi l'AOP LTC1050 prenant en entrée un courant faible (de l'ordre d'une dizaine de picoampère).

<p align="center">
<img src=https://github.com/MOSH-Insa-Toulouse/2024-2025-4GP-divdav7-EnzoJ26/blob/main/Images_Projet/Circuit%20LTSpice.JPG alt="Circuit amplificateur réalisé dans LTSpice">
<br>
<i>Circuit amplificateur réalisé dans LTSpice</i>
</p>
<br>
Ce circuit amplificateur contient également 3 filtres passe bas passifs, dont nous avons pu tester le bon fonctionnement alternativement.<br>
<br>
- Le premier étage a une fréquence de coupure de 16 Hz, il permet de filtrer les bruits du courant en entrée. Nous avons testé ce premier étage pour vérifier que la fréquence de coupure soit la bonne.
<br>
</p>

<p align="center">
<img src=https://github.com/MOSH-Insa-Toulouse/2024-2025-4GP-divdav7-EnzoJ26/blob/main/Images_Projet/Simulation%20%C3%A9tage%201.JPG alt="Simulation de l'étage 1 dans LTSpice">
<br>
<i>Simulation de l'étage 1 dans LTSpice</i>
</p>
En effet la fréquence de coupure est la bonne.
<br>
- Le second étage a une fréquence de coupure de 1,6 Hz. Il permet de filtrer la composante de bruit du réseau électrique: le bruit à 50 Hz.
<br>
</p>
<p align="center">
<img src=https://github.com/MOSH-Insa-Toulouse/2024-2025-4GP-divdav7-EnzoJ26/blob/main/Images_Projet/Simulation%20%C3%A9tage%202.JPG alt="Simulation de l'étage 2 dans LTSpice">
<br>
<i>Simulation de l'étage 2 dans LTSpice</i>
</p>
En effet la fréquence de coupure est la bonne.
<br>
- Le dernier étage a une fréquence de coupure à 1,6 kHz qui permet d'atténuer le bruit provenant lors du traitement électronique.
<br>
</p>
<p align="center">
<img src=https://github.com/MOSH-Insa-Toulouse/2024-2025-4GP-divdav7-EnzoJ26/blob/main/Images_Projet/Simulation%20%C3%A9tage%203.JPG alt="Simulation de l'étage 3 dans LTSpice">
<br>
<i>Simulation de l'étage 3 dans LTSpice</i>
</p>
En effet la fréquence de coupure est la bonne.
<br> 
Conclusion : grâce à la simulation sur LTSpice, nous remarquons que notre signal de sortie atteindra les 1V, ce qui lui permet d'être récupéré et exploité par la carte arduino UNO.

## IV) KiCad et PCB

Avant d'imprimer le PCB et de souder nos différents composants, nous avons dû dessiner les schémas électroniques, réaliser les composants électroniques et créer le PCB sur KICAD. Cette étape est importante car elle permet de prévisualiser le PCB, vérifier que tous les composants tiennent sur le PCB. Elle permet également de créer les différents routages de pistes.

<p align="center">
<img src=https://github.com/MOSH-Insa-Toulouse/2024-2025-4GP-divdav7-EnzoJ26/blob/main/Images_Projet/Sch%C3%A9matique%20KiCad.JPG alt="Schémas électroniques">
<br>
<i>Schémas électroniques</i>
</p>

<p align="center">
<img src=https://github.com/MOSH-Insa-Toulouse/2024-2025-4GP-divdav7-EnzoJ26/blob/main/Images_Projet/PCB%20editeur%20de%20PCB.JPG alt="Création du PCB et des différentes empreintes sur KiCad">
<br>
<i>Création du PCB et des différentes empreintes sur KiCad</i>
</p>

<p align="center">
<img src=https://github.com/MOSH-Insa-Toulouse/2024-2025-4GP-divdav7-EnzoJ26/blob/main/Images_Projet/Mod%C3%A8le%203D%20du%20PCB.JPG alt="Visualisation 3D du PCB">
<br>
<i>Visualisation 3D du PCB</i>
</p>
<br>
Une fois que nous avons été satisfaites de la disposition de nos différents composants électroniques, et du routage de pistes, nous avons pu imprimer le PCB, le percer et y souder les différents composants.
<br>
<br>
<p align="center">
<img src=https://github.com/MOSH-Insa-Toulouse/2024-2025-4GP-divdav7-EnzoJ26/blob/main/Images_Projet/PCB%20termin%C3%A9.JPG alt="PCB terminé">
<br>
<i>PCB terminé</i>
</p>


## IV) Code Arduino

Le code Arduino permet la réception des mesures faites par les deux capteurs (Graphites et Flex Sensor) et d'afficher ces valeurs sur l'OLED. Pour cela nous nous sommes inspirés des codes disponibles en open-source. Du côté de l'application, bien qu'elle soit fonctionnelle sur la partie connexion au module bluetooth, elle ne trace pas le graphe de la variation de la résistance de nos capteurs en fonction de la déformation. En attendant de pouvoir déboguer le problème lié au code, nous avons donc pu effectuer nos mesures gräce à l'écran OLED. 
Notre programme principal est composé de sous programmes associés à ces éléments (à noter que nous ne nous sommes pas servis du servo-moteur dans ce projet):
<br>
<p align="center">
<img src=https://github.com/MOSH-Insa-Toulouse/2024-2025-4GP-divdav7-EnzoJ26/blob/main/Images_Projet/Modules%20Arduino.JPG alt="Figure 2 - Capteur à jauge de contrainte à base de crayon graphite">
<br>
<i>Différents modules arduino</i>
<br>   

Les objectifs de la partie Arduino étaient d'acquérir les données de nos capteurs (résistance en fonction de la déformation), d'afficher nos mesures sur l'écran OLED via un menu déroulant contrôlé par l'encodeur et enfin, de communiquer avec l'application créée sous MIT APP Inventor. Ils ont été dans l'ensemble bien remplis.






