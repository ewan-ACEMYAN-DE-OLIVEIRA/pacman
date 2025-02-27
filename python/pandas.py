import pandas
import matplotlib.pyplot as plt
#1
COVID=pandas.read_csv("owid-covid-data.csv")
#2
COVID_NC=COVID["new_cases"]
print(COVID_NC.head(8))
#3
COVID2102=COVID.loc[COVID['date']=='2021-02-01']
affichage=COVID2102["new_cases"]
print("Nombre de patients hospitalisés le 1 Février 2021: ",affichage)
print("Nombre de patients hospitalisés le 1 Février 2021: ",affichage.values[0])
#4
print("Nombre moyens de patient hospitalisés",COVID["hosp_patients"].mean())
print("indicateurs statistiques du nombre de patients hospitalisés")
print(COVID["hosp_patients"].describe())
#5
COVID_2021=COVID.loc[(COVID['date']>='2021-01-01')&(COVID['date']<='2021-12-31')]
print("Indicateurs statistiques du nombre de patients hospitalisés sur l'année 2021")
print(COVID_2021["hosp_patients"].describe())
#6
COVID['date']=pandas.to_datetime(COVID['date'])
COVID_2021_new=COVID.loc[COVID['date'].dt.year==2021]
print("Indicateur statistiques du nombre de patients hospitalisés sur l'année 2021")
print(COVID_2021_new["hosp_patients"].describe())
#7
COVID_2021_new["hosp_patients"].plot.box(whis=[0,100],vert=False)
plt.title('Diagramme en boite des hospitalisations COVID en 2021')
plt.show()
#8
COVID.plot(x='date',y='new_cases_smoothed',figsize=(8,8))
plt.show()
#9
COVID.loc[COVID['date']<='2021-11-30'].plot(x='date', y=['new_cases_smoothed','hosp_patients','icu_patients'],secondary_y='icu_patients', figsize=(8,8))
plt.show()
#10
COVID.loc[(COVID['date']>='2021-11-01')&(COVID['date']<='2021-12-15')].plot.bar(x='date',y=['new_cases_smoothed','hosp_patients'],figsize=(8,8))
plt.show()
