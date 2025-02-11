# Exercice 1

from math import sqrt

def calculMoyenne(x):
    somme = 0 
    for elt in x :
        somme += elt 
    return somme/len(x)

def calculEcartType(x):
    moyenne = calculMoyenne(x)
    variance = 0
    for elt in x :
        variance += (elt - moyenne)**2
    return sqrt(variance)


notes = [8,17,6,8,20,12.5,7.5,6,12]
liste = [1,2,3,4,5,6,7,8,9,10]

print("La moyenne des notes est :",calculMoyenne(notes))
print("L'écart-type des notes est :",calculEcartType(notes))

# Exercice 2

def calculMediane(x):
    x.sort()
    if len(x) % 2 == 0 :
        mediane = (x[len(x)//2] + x[(len(x)//2)-1]) / 2
    else:
        mediane = x[len(x)//2]
    return mediane

def calculMedianeV2(x):
    x.sort()
    serie1 = []
    serie2 = []
    if len(x) % 2 == 0 :
        mediane = (x[len(x)//2] + x[(len(x)//2)-1]) / 2
        for i in range(0,len(x)//2):
            serie1.append(x[i])
        for i in range(len(x)//2,len(x)):
            serie2.append(x[i])
    else:
        mediane = x[len(x)//2]
        for i in range(0,len(x)//2):
            serie1.append(x[i])
        for i in range(len(x)//2+1,len(x)):
            serie2.append(x[i])
    return mediane, serie1, serie2

def calculQartiles(x):
    liste = calculMedianeV2(x)
    q1 = calculMediane(liste[1])
    q2 = liste[0]
    q3 = calculMediane(liste[2])
    return q1,q2,q3

    
    
print("------------------")

print("La médiane de la liste est :",calculMediane(liste))
print("La médiane des notes est :",calculMediane(notes))

print(calculMedianeV2(liste))
print(calculMedianeV2(notes))

print("Les quartiles q1 q2 q3 de la liste sont respectivement :",calculQartiles(liste))
print("Les quartiles q1 q2 q3 des notes sont respectivement :",calculQartiles(notes))

# Exercice 3 

def maximum(x):
    y = []
    for elt in x :
        y.append(elt)
    y.sort()
    return y[-1]

def modalitesEtEffectifs(x):
    modalites = []
    effectifs = []
    for elt in x :
        if elt not in modalites :
            modalites.append(elt)
    modalites.sort()
    for elt in modalites :
        count = 0
        for element in x :
            if element == elt :
                count +=1
        effectifs.append(count)
    return modalites, effectifs
                
def afficherTableauStatistique(x):
    liste = modalitesEtEffectifs(x)
    modalites = liste[0]
    effectifs = liste[1]
    frequences = []
    effCumm = []
    freqCumm = []
        
    for elt in effectifs :
        frequences.append(elt/len(x))
        
    for i in range(0,len(effectifs)) :
        if i == 0 :
            effCumm.append(effectifs[i])
        else :
            effCumm.append(effectifs[i]+effCumm[i-1])
    
    for i in range(0,len(frequences)) :
        if i == 0 :
            freqCumm.append(frequences[i])
        else :
            freqCumm.append(frequences[i]+freqCumm[i-1])
    
        
    return modalites, effectifs, frequences, effCumm, freqCumm

def calculMode(x) :
    tabStatistique = afficherTableauStatistique(x)
    maxFreq = maximum(tabStatistique[2])
    modes = []
    indice = -1
    for elt in tabStatistique[2] :
        indice += 1
        if elt == maxFreq :
            modes.append(tabStatistique[0][indice])
    return modes
    
liste2 = [1,1,2,2,3,4,5,7,9,9]

print("------------------")

print("La note maximale est :",maximum(notes))
print("Liste des modalités et des effectifs des notes :",modalitesEtEffectifs(notes))
print("")
print("Liste2 :", liste2)
print("Tableau statistique de liste2 :",afficherTableauStatistique(liste2))
print("Modalités de liste2 :",calculMode(liste2))
