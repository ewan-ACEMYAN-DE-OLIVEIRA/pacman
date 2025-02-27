
import pandas
import matplotlib.pyplot as plt 
parcoursup  = pandas.read_csv("Parcoursup.csv")

print("Indicateurs des donnés statistiques du % admises:\n",parcoursup["%Admises"].describe())

Aix = parcoursup.loc[parcoursup["Code UAI"]=='013406IU']

print("Nombre moyen de candidats à Aix ",Aix["Total candidats"].mean())

print("indicateur statistiques du % d'admises à Aix\n", Aix ["%Admises"].describe())

Aix.pie(y = "%Admises" ; labels = Aix["Session"], legend = None, title = "Diagramme circulaire du % d'étudiantes admises selon la sessions")
