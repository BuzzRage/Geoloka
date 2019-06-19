import numpy as np
import matplotlib.pyplot as plt
import os
import pandas as pd
from scipy.stats import ks_2samp
import gmplot
import numpy as np
import seaborn as sns
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
data_csv = pd.read_csv(os.path.join(os.getcwd(), 'vdb_fixe.csv'), names=['Date', 'Heure', 'Latitude', 'Longitude','Altitude','NBsat','HDOP'])
#enregistre des donnees 1
data_date1 = data_csv['Date'].values[1:]
data_heure1 = list(data_csv['Heure'].values[1:])
data_heure1=[ x.split(":") for x in data_heure1]
data_second1=[ int(data_heure1[i][0])*60*60+int(data_heure1[i][1])*60+int(data_heure1[i][2]) for i in range(len(data_heure1))]
data_second1=[ x-data_second1[0] for x in data_second1]
data_lat1 = [float(x) for x in data_csv['Latitude'].values[1:]]
data_long1 = [float(x) for x in data_csv['Longitude'].values[1:]]
data_alt1 = [float(x) for x in data_csv['Altitude'].values[1:]]

data_x=[lat2cart(data_long1[i],data_lat1[i],data_alt1[i])[0] for i in range(len(data_lat1))]
data_y=[lat2cart(data_long1[i],data_lat1[i],data_alt1[i])[1] for i in range(len(data_lat1))]
data_z=[lat2cart(data_long1[i],data_lat1[i],data_alt1[i])[2] for i in range(len(data_lat1))]

list_dist=[]
for i in range(len(data_lat1)-1):
	dist=np.sqrt(pow(data_lat1[i+1]-data_lat1[i],2)+pow(data_long1[i+1]-data_long1[i],2))
	list_dist.append(dist);
	
print(list_dist)
print(len(list_dist))
DeltaTime=[data_second1[i+1]-data_second1[i]+2 for i in range(len(data_second1)-1)]
print(DeltaTime)
print(len(DeltaTime))
vitesse=[list_dist[i]/float(DeltaTime[i]) for i in range(len(DeltaTime))]
#data_to_plot=[list_dist]


fig = plt.figure()

# Create an axes instance
ax = fig.add_subplot(131)
bp = ax.boxplot(data_x)
plt.grid()
plt.ylabel("x")
plt.xlabel("Maison")

ax = fig.add_subplot(132)
bp = ax.boxplot(data_y)
plt.grid()
plt.ylabel("y")
plt.xlabel("Maison")

ax = fig.add_subplot(133)
bp = ax.boxplot(data_z)
plt.grid()
plt.ylabel("z")
plt.xlabel("Maison")

#on enregistre le graph
#plt.savefig('boxplot-predict_testing.png')
plt.show()
