2024-2025-4GP-HILLAH-JACQUET
---
## Sommaire 
   - [I) Contexte et objectifs du projet](#I-Contexteetobjectifsduprojet)
   - [II) Capteur à base de crayon graphite](#II-Capteuràbasedecrayongraphite)
   - [III) Electronique Analogique](#III-Electroniqueanalogique)
   - [IV) KiCad et PCB](#IV-KiCadetPCB)
   - [V) Arduino](#V-Arduino)
   - [VI) Application Mobile](#VI-ApplicationMobile)
   - [VII) Banc de Test](#VII-BancdeTest)
   - [VIII) Datasheet](#VIII-Datasheet)

---

## I) Contexte et objectifs du projet

L'objectif de ce projet est de réaliser un capteur low-tech à base de graphite sachant que les recherches ont démontré les nombreux avantages de l'utilisation de ce matériau. L’électronique à base de papier gagne en popularité parmi les ingénieurs grâce à sa facilité d’approvisionnement, sa simplicité de fabrication et son faible coût.<br>
Pour ce faire, nous avons mis en place la réalisation de tous les processus de fabrication d'un shield PCB que nous avons ensuite connecté à une carte Arduino.<br>
Notre PCB contient 6 composants différents : une jauge de contrainte, un flex sensor, un écran oled, un encodeur rotatoire, un potentiomètre digital et un module Bluetooth. À noter que nous n'avons pas utilisé le servo-moteur qui faisait partie des composants disponibles à implémenter.<br>
La carte Arduino fonctionne avec un code que nous avons implémenté en s'inspirant grandement des codes qui étaient disponibles en open-source.<br>
Nous avons ensuite réalisé une application mobile connectée au module bluetooth afin d'acquérir les données renvoyées par nos capteurs sur le téléphone.<br>
Nous avons évalué le capteur sur un banc de test avant de conclure par la datasheet de notre capteur.<br>
À la fin de la lecture de ce github, vous devriez savoir quel capteur choisir entre le graphite et le flex ;) . <br>

# Livrables

- PCB shield pour arduino uno <br>
- Code arduino <br>
- Banc de test du capteur <br>
- Application android qui fonctionne par bluetooth <br>
- Datasheet <br>

# Matériel utilisé

-Résistances: 1x1kΩ, 1x10kΩ, 2x100 kΩ, 1x47kΩ <br>
-Potentiomètre digital MCP41050 <br>
-Capacités: 3x100nF et 1x1µF <br>
-Arduino Uno <br>
-Capteur graphite à partir d'un support papier et d'un crayon <br>
-Flex Sensor <br>
-Encodeur rotatoire Keyes KY-040 <br>
-Ecran OLED 128x64 <br>
-Module Bluetooth HC-05 <br>
-Amplificateur opérationnel LTC1050 <br>

## II) Capteur à base de crayon graphite

Ce capteur low-tech utilise la théorie de la percolation au sein d’un système granulaire constitué de nanoparticules de graphite.<br>
Nous déposons une fine couche de graphite sur la feuille de papier ce qui présente un réseau percolé caractéristique. Dans ce réseau, le transport des électrons entre les nanoparticules est basé sur le mécanisme de l’effet tunnel.<br>
Lors de l'application de déformations en tension, le réseau percolé se trouve directement étendu. Cette déformation entraîne une augmentation de la distance effective entre les particules de graphite au sein du réseau. Une partie des chemins de percolation sont par conséquent rompus. Ainsi, la conduction du matériau diminue, donc la résistance de la couche de graphite augmente.<br>
Inversement, lors de l’application de déformations en compression, le réseau percolé se trouve directement comprimé. Cette déformation entraîne une diminution de la distance effective entre les particules.<br> 
Ainsi, de nouveaux chemins de percolation sont créés, la conduction du matériau augmente, soit la résistance de la couche de graphite diminue.

Voici un schéma explicatif de ces phénomènes:

<p align="center">
<img src=https://github.com/MOSH-Insa-Toulouse/2024-2025-4GP-divdav7-EnzoJ26/blob/main/Images_Projet/Frise%20duret%C3%A9%20mine%20des%20crayons.JPG alt="Frise des duretés">
<br>
<i>Sensibilité et Résistance en fonction de la dureté des mines</i>
</p>

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
Pour obtenir un signal exploitable nous devions l'amplifier. Pour ce faire, nous avons utilisé un montage transimpédance constitué d'un amplificateur opérationnel.<br>
Pour cela nous avons dans un premier temps effectué une simulation électronique du capteur et de ce montage transimpédance sous LTSpice. Voici ci-dessous le montage réalisé sous LTSpice. Pour notre montage nous avons choisi l'AOP LTC1050 prenant en entrée un courant faible (de l'ordre d'une dizaine de picoampère).

<p align="center">
<img src=https://github.com/MOSH-Insa-Toulouse/2024-2025-4GP-divdav7-EnzoJ26/blob/main/Images_Projet/Circuit%20LTSpice.JPG alt="Circuit amplificateur réalisé dans LTSpice">
<br>
<i>Circuit amplificateur réalisé dans LTSpice</i>
</p>
<br>
Ce circuit amplificateur contient 3 filtres passe bas passifs, dont nous avons pu tester le bon fonctionnement.<br>
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
La fréquence de coupure est de 2,1Hz alors que celle attendue est de 1,6Hz, ce qui reste dans les plages de fréquences attendues, donc nous la validons.
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
Cette simulation nous permet de confirmer que l’arduino pourra bel et bien mesurer le signal de sortie car il sera d’environ 1V.


## IV) KiCad et PCB

Avant d'imprimer le PCB et de souder nos différents composants, nous avons dessiné les schémas électroniques, réalisé les composants électroniques et créé le PCB sur KICAD. Cette étape est importante car elle permet de prévisualiser le PCB, vérifier que tous les composants tiennent sur le PCB et ainsi de le sortir. Elle permet également de créer les différents routages de pistes. 

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
<i>Visualisation 3D du PCB (idéale)</i>
</p>
<br>
Lorsque la disposition de nos différents composants électroniques était terminée suite au routage des pistes, nous avons pu imprimer le PCB, le percer et y souder les différents composants.
<br>
<br>
<p align="center">
<img src=https://github.com/MOSH-Insa-Toulouse/2024-2025-4GP-divdav7-EnzoJ26/blob/main/Images_Projet/PCB%20termin%C3%A9.JPG alt="PCB terminé">
<br>
<i>PCB terminé</i>
</p>

Nous avons rencontré des problèmes lors de cette étape qui représentait une étape très importante du projet. Les librairies étaient assez complexes à retrouver et comme elles contenaient nos différentes empreintes cela nous a posé pas mal de problèmes dans notre gestion du temps, mais également dans la difficulté que l'on a éprouvé à en refaire.<br>
Au niveau du routage, il fallait optimiser la place présente sur le PCB tout en choisissant stratégiquement l'emplacement des différents modules. Cela n'a pas pu se faire sans la présence de 3 via. La visualisation 3D de notre PCB est bien celle qui découle de notre éditeur de PCB, mais c'était sans compter sur le fait que certains des modules que nous avions avaient leurs broches en déphasage avec les connexions disponibles présentes sur notre PCB (le module Bluetooth a une broche dans le vide et l'écran OLED se connecte de sorte à ne pas laisser la place au Flex Sensor de se connecter sur le PCB directement. Nous avons donc réfléchi à une solution filaire permettant de connecter l'écran OLED tout en le gardant à l'extérieur de la carte). Les autres modules n'ont pas de soucis de connexion.<br>
Nous avons solicité l'aide de Mr Grisolia et de Cathy particulièrement sur la partie empreintes et sur le routage. Nous les remercions de nous avoir débloqué plus d'une fois.

## V) Code Arduino

Le code Arduino permet la réception des mesures faites par les deux capteurs (Graphites et Flex Sensor) et d'afficher ces valeurs sur l'OLED. Pour cela nous nous sommes inspirés des codes disponibles en open-source. Du côté de l'application, bien qu'elle soit fonctionnelle sur la partie connexion au module bluetooth, elle ne trace pas le graphe de la variation de la résistance de nos capteurs en fonction de la déformation. En attendant de pouvoir déboguer le problème lié au code, nous avons donc pu effectuer nos mesures gräce à l'écran OLED et l'encodeur rotationnel qui permet de sélectionner le menu voulu par l'utilisateur. À noter l'utilisation d'un potentiomètre digital qui sert particulièrement à faire varier la valeur de la résistance R2 afin de mieux gérer notre gain. 
Notre programme principal est composé de sous programmes associés à ces éléments (à noter que nous ne nous sommes pas servis du servo-moteur dans ce projet):
<br>
<p align="center">
<img src=https://github.com/MOSH-Insa-Toulouse/2024-2025-4GP-divdav7-EnzoJ26/blob/main/Images_Projet/Modules%20Arduino.JPG alt="Figure 2 - Capteur à jauge de contrainte à base de crayon graphite">
<br>
<i>Différents modules arduino</i>
<br>   

Les objectifs de la partie Arduino étaient d'acquérir les données de nos capteurs (résistance en fonction de la déformation), d'afficher nos mesures sur l'écran OLED via un menu déroulant contrôlé par l'encodeur et enfin, de communiquer avec l'application créée sous MIT APP Inventor. Ils ont été dans l'ensemble bien remplis.

## VI) Application mobile

Nous avons réalisé une application mobile sur le site MIT APP Inventor. <br>
Une fois que le module bluetooth est connecté au téléphone, nous pouvons acquérir les données des capteurs en temps réel et les tracer sur le graphe. 

<br> 
<p align="center">
<img src=https://github.com/MOSH-Insa-Toulouse/2024-2025-4GP-divdav7-EnzoJ26/blob/main/Images_Projet/Simulation%20sur%20MIT%20APP%20Inventor.JPG alt="Simulation sur MIT APP Inventor">
<br> 
<i>Aperçu MIT APP Inventor</i>
<br>
<p align="center">
<img src=https://github.com/MOSH-Insa-Toulouse/2024-2025-4GP-divdav7-EnzoJ26/blob/main/Images_Projet/Face%20avant%20application.jpeg alt="Face avant de l'application">
<br> 
<i>Face avant de l'application</i>
<br>
Malgré une application fonctionnelle sur la partie de recherche des périphériques et sur la connexion au HC-05, elle présente des limites sur le tracé de la variation de la résistance de notre capteur en fonction de la déformation qui y est appliquée. Comme détaillé lors de la partie précédente, en attendant de débuguer le problème, nous avons codé l'écran OLED pour qu'il nous affiche les valeurs d'intérêt.
   
## VII) Banc de tests

Nous avons relevé la variation de résistance de notre capteur en fonction de son angle de déformation. Pour cela, nous nous sommes servis de ce modèle de banc de test réalisé au FabLab, possédant pour chaque étage la même épaisseur, mais un diamètre différent. Nous disposions de 5 étages allant de 2 à 4 cm de diamètre avec un pas de 0,5cm entre chaque étage, tous étant épais de 0,2cm.
<br> 
<p align="center">
<img src=https://github.com/MOSH-Insa-Toulouse/2024-2025-4GP-divdav7-EnzoJ26/blob/main/Images_Projet/Banc%20de%20test.JPG alt="Banc de test">
<br>
<i>Banc de test utilisé</i>
<br>
Les objectifs du banc étaient de pouvoir enregistrer les données, les traiter, les comparer avant de pouvoir concevoir et rédiger notre Datasheet.
   
## VIII) Datasheet

Nous avons déposé le PDF de la datasheet contenant une conclusion sur le choix de nos capteurs en fonction des résultats que nous avions obtenus que nous vous invitons à aller voir. Nous vous alertons sur le fait que nous avons donné un modèle linéaire aux différentes mesures que nous avons obtenu en compression et en tension, malgré le fait que les courbes de base ne soient pas linéaires. Les sensibilités que nous calculons sont obtenues en prenant en compte nos modèles linéaires. <br>

# Contacts
<br>
Si vous avez des questions sur ce projet, n'hésitez pas à contacter notre superbe duo (le binôme de l'année on a dit): 
<br>

David : hillah-a@insa-toulouse.fr
<br>
Enzo : ejacquet@insa-toulouse.fr


