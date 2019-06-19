import numpy as np
import matplotlib.pyplot as plt
import os
import pandas as pd
from scipy.stats import ks_2samp
import gmplot
import numpy
from statistics import mean 
from math import sqrt
from math import sin,cos,atan, degrees, radians,atan2,log,tan,pi

a = 6378157.0
b = 6356752.3142
e = sqrt((a**2 - b**2)/a**2)
def lat2cart(longi,lati,h):
	longi = radians(longi)
	lati = radians(lati)

	N = a/(sqrt(1-(e**2)*(sin(lati))**2))

	x = (N+h)*cos(lati)*cos(longi)
	y = (N+h)*cos(lati)*sin(longi)
	z = (N*(1-e**2)+h)*sin(lati)

	return (x,y,z,N)

#chargement csv
data_csv = pd.read_csv(os.path.join(os.getcwd(), 'utt-feragus.csv'), names=['Date', 'Heure', 'Latitude', 'Longitude','Altitude','NBsat','HDOP'])
#enregistre des angles
data_date = data_csv['Date'].values[1:]
data_heure = list(data_csv['Heure'].values[1:])
data_heure=[ x.split(":") for x in data_heure]
data_second=[ int(data_heure[i][0])*60*60+int(data_heure[i][1])*60+int(data_heure[i][2]) for i in range(len(data_heure))]
data_second=[ x-data_second[0] for x in data_second]
data_lat = [float(x) for x in data_csv['Latitude'].values[1:]]
data_long = [float(x) for x in data_csv['Longitude'].values[1:]]
data_alt = [float(x) for x in data_csv['Altitude'].values[1:]]

data_x=[lat2cart(data_long[i],data_lat[i],data_alt[i])[0] for i in range(len(data_lat))]
data_y=[lat2cart(data_long[i],data_lat[i],data_alt[i])[1] for i in range(len(data_lat))]
data_z=[lat2cart(data_long[i],data_lat[i],data_alt[i])[2] for i in range(len(data_lat))]


list_dist=[]
for i in range(len(data_x)-1):
	dist=np.sqrt(pow(data_x[i+1]-data_x[i],2)+pow(data_y[i+1]-data_y[i],2))
	list_dist.append(dist);
	
print(list_dist)
Distance=sum(list_dist)

print("Distance :" + str(Distance))

DeltaTime=[data_second[i+1]-data_second[i] for i in range(len(data_second)-1)]
print(sum(DeltaTime))

vitesse=[list_dist[i]/float(DeltaTime[i]) for i in range(len(DeltaTime))]
moyenne=[mean(vitesse) for i in range(len(DeltaTime))]

print(mean(vitesse))
plt.subplot(211)
plt.plot(data_second[:-1],list_dist,'b-',label='Distance')
plt.grid()
plt.ylabel("Distance(m)")
plt.xlabel("Temps(s)")

plt.subplot(212)
plt.plot(data_second[:-1],vitesse,'b-',label='Vitesse')
plt.plot(data_second[:-1],moyenne,'r-',label='moyenne')
plt.grid()
plt.ylabel("Vitesse(m/s)")
plt.xlabel("Temps(s)")
plt.show()


