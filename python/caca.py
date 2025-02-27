#Exercice 1

import pandas
import matplotlib.pyplot as plt
parcoursup = pandas.read_csv("Parcoursup.csv")

#1

print("Indicateurs des données statistiques du % admises:\n", parcoursup["%Admises"].describe())

#2

Aix = parcoursup.loc[parcoursup["Code UAI"]=='0134061U']

#a

print("Nombre moyen de candidats à Aix :", Aix["Total candidats"].mean())

#b

print("Indicateur statistiques du % d'admises à Aix:\n", Aix ["%Admises"].describe())

#c 

Aix.plot.pie(y = "%Admises" , labels = Aix["Session"], legend = None, title = "Diagramme circulaire du % d'étudiantes admises selon les sessions")
plt.show()
