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

#d 

Aix.plot.bar(x = "Session", y =["Total candidats"], secondary_y = "Candidats", width = 0.5, edgecolor = 'black')
plt.title("Evolution du nombre de candidats / Candidates")
plt.show()

# 3
# a

parcoursup2021 = parcoursup .loc[parcoursup["Session"]==2021]
M = max(parcoursup2021["%Admises"])
ville_max = parcoursup2021.loc[parcoursup2021["%Admission"]==M]["Etablissement"]
print("En 2021, le % d'admission maximal était de ", M, "atteint à", ville_max.values[0])
m = min(parcoursup2021["%Admission"])
ville_min = parcoursup2021.loc[parcoursup2021["%Admission"]==m][["Etablissement","code UAI"]]
print("En 2021, le % d'admission maximal était de", m,"atteint à", ville_min["Etablissement"].values[0])
