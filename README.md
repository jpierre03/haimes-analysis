# HAIMES Analysis

Cet outil génère une image (un semblable de diagramme de Gantt) à partir d'un résulat de simulation d'un atelier de production.

## Format du fichier d'entrée

* le fichier utilisé est de type `csv`
* une ligne par enregistrement

Exemple de ligne : 

```plain
	order2-0,4,M5,471,525,600,600
```

Ce qui correspond à :

* identifiant ordre de frabrication,
* numéro d'étape de fabrication,
* identifiant du poste de travail,
* date d'arrivée sur le poste de travail (nombre),
* date de début de travail (nombre),
* date de fin de travail (nombre),
* date de sortie du poste de travail

