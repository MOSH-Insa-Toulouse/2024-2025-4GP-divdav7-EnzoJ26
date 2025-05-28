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

## II)Capteur à jauge de contrainte à base de crayon graphite

Ce capteur low-tech utilise la théorie de la percolation au sein d’un système granulaire constitué de nanoparticules de graphite.<br>
Nous déposons une fine couche de graphite sur la feuille de papier ce qui présente un réseau percolé caractéristique. Dans ce réseau, le transport des électrons entre les nanoparticules est basé sur le mécanisme de l’effet tunnel.<br>
Lors de l'application de déformations en tension, le réseau percolé se trouve directement étendu. Cette déformation entraîne une augmentation de la distance effective entre les particules de graphite au sein du réseau. Une partie des chemins de percolation sont par conséquent rompus. Ainsi, la conduction du matériau diminue, donc la résistance de la couche de graphite augmente.<br>
Inversement, lors de l’application de déformations en compression, le réseau percolé se trouve directement comprimé. Cette déformation entraîne une diminution de la distance effective entre les particules.<br> Ainsi, de nouveaux chemins de percolation sont créés, la conduction du matériau augmente, soit la résistance de la couche de graphite diminue.

Une jauge de contrainte est un circuit résistif dont la résistance varie en fonction de sa déformation. Donc en mesurant les variations de résistance de la jauge, nous en avons déduit la contrainte et la déformation appliquées.<br>
Notre jauge de contrainte est alimentée par une tension régulée de 5V d'une carte Arduino UNO.
<br>
Voici ci dessous le schéma d'un modèle de capteur graphite. Dans notre projet, nous avons utilisé les crayons de dureté 2H ; 2B ; HB.

<p align="center">!















