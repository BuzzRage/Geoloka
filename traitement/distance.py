import numpy as np
import matplotlib.pyplot as plt
import os
import pandas as pd
from scipy.stats import ks_2samp
import gmplot
import numpy as np





#chargement csv
data_csv = pd.read_csv(os.path.join(os.getcwd(), '3.csv'), names=['Date', 'Heure', 'Latitude', 'Longitude','Altitude'])
#enregistre des donnees 1
data_date1 = data_csv['Date'].values[1:]
data_heure1 = list(data_csv['Heure'].values[1:])
data_heure1=[ x.split(":") for x in data_heure1]
data_second1=[ int(data_heure1[i][0])*60*60+int(data_heure1[i][1])*60+int(data_heure1[i][2]) for i in range(len(data_heure1))]
data_second1=[ x-data_second1[0] for x in data_second1]
data_lat1 = [float(x) for x in data_csv['Latitude'].values[1:]]
data_long1 = [float(x) for x in data_csv['Longitude'].values[1:]]
data_alt1 = [float(x) for x in data_csv['Altitude'].values[1:]]


list_dist=[]
for i in range(len(data_lat1)-1):
	dist=np.sqrt(pow(data_lat1[i+1]-data_lat1[i],2)+pow(data_long1[i+1]-data_long1[i],2))
	list_dist.append(dist);
	
print(list_dist)
print(len(list_dist))
DeltaTime=[data_second1[i+1]-data_second1[i] for i in range(len(data_second1)-1)]
print(DeltaTime)
print(len(DeltaTime))
vitesse=[list_dist[i]/float(DeltaTime[i]) for i in range(len(DeltaTime))]
#data_to_plot=[list_dist]


fig = plt.figure()

# Create an axes instance
ax = fig.add_subplot(131)
bp = ax.boxplot(data_lat1)
plt.grid()
plt.ylabel("Latitude")
plt.xlabel("Cas 1")

ax = fig.add_subplot(132)
bp = ax.boxplot(data_long1)
plt.grid()
plt.ylabel("Longitude")
plt.xlabel("Cas 1")

ax = fig.add_subplot(133)
bp = ax.boxplot(data_alt1)
plt.grid()
plt.ylabel("Altitude")
plt.xlabel("Cas 1")

#on enregistre le graph
#plt.savefig('boxplot-predict_testing.png')
plt.show()

#on affiche les courbes
#plt.subplot(311)

#plt.plot(vitesse,[ i for i in range(len(data_lat)-1)],'b-',label='Latitude')
#plt.plot(data_second,data_lat,'b-',label='Latitude')
plt.grid()
#plt.ylabel("Coordonnees")
#plt.xlabel("Temps(s)")
#plt.xlim(0,139)
#plt.subplot(312)
#plt.plot(data_long,[ i for i in range(len(data_long))],'r-',label='Longitude')
#plt.plot(data_second,data_long,'r-',label='Longitude')
#kstest=ks_2samp(data_steer,data_list)
#print (kstest)
#legend
#plt.legend(bbox_to_anchor=(0., 1.02, 1., .102), loc=3,ncol=2, mode="expand", borderaxespad=0.)
#plt.grid()
#plt.ylabel("Coordonnees")
#plt.xlabel("Temps(s)")

#plt.subplot(313)
#plt.plot(data_lat,data_long,'b-')
#plt.grid()
#plt.ylabel("Longitude")
#plt.xlabel("Latitude")
#plt.xlim(0,139)
#on enregistre le graph
#plt.savefig('2.png')
plt.show()

#gmap3 = gmplot.GoogleMapPlotter(data_lat[1],data_long[1] , 13) 
#gmap3.apikey="AIzaSyASykRT1nBDi3zDKbHYpmSy1nTMagWObLI"
#gmap3.scatter( data_lat, data_long, '# FF0000',size = 40, marker = False ) 
  
# Plot method Draw a line in 
# between given coordinates 
#gmap3.plot(data_lat, data_long,'cornflowerblue', edge_width = 2.5) 
  
#gmap3.draw( "map1.html" ) 

