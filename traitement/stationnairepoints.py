import numpy as np
import matplotlib.pyplot as plt
import os
import pandas as pd
from scipy.stats import ks_2samp
import gmplot
import numpy

#chargement csv
data_csv = pd.read_csv(os.path.join(os.getcwd(), 'colline-fixe.csv'), names=['Date', 'Heure', 'Latitude', 'Longitude','Altitude','NBsat','HDOP'])
#enregistre des angles
data_date = data_csv['Date'].values[1:]
data_heure = list(data_csv['Heure'].values[1:])
data_heure=[ x.split(":") for x in data_heure]
data_second=[ int(data_heure[i][0])*60*60+int(data_heure[i][1])*60+int(data_heure[i][2]) for i in range(len(data_heure))]
data_second=[ x-data_second[0] for x in data_second]
data_lat = [float(x) for x in data_csv['Latitude'].values[1:]]
data_long = [float(x) for x in data_csv['Longitude'].values[1:]]
data_alt = [float(x) for x in data_csv['Altitude'].values[1:]]


#on affiche Latitude

plt.subplot(411)
#plt.plot(data_lat,[ i for i in range(len(data_lat))],'b*',label='Latitude')
plt.plot(data_second,data_lat,'b-',label='Latitude')
plt.grid()
plt.ylabel("Latitude")
plt.xlabel("Temps(s)")
#plt.xlim(0,139)

#on affiche Longitude

plt.subplot(412)
#plt.plot(data_long,[ i for i in range(len(data_long))],'r*',label='Longitude')
plt.plot(data_second,data_long,'r-',label='Longitude')

#legend
#plt.legend(bbox_to_anchor=(0., 1.02, 1., .102), loc=3,ncol=2, mode="expand", borderaxespad=0.)
plt.grid()
plt.ylabel("Longitude")
plt.xlabel("Temps(s)")

#on affiche altitude

plt.subplot(413)
plt.plot(data_second,data_alt,'y*')
plt.grid()
plt.ylabel("Altitude")
plt.xlabel("Temps(s)")

#on affiche long/alt

plt.subplot(414)
plt.plot(data_lat,data_long,'g*')
plt.grid()
plt.ylabel("Longitude")
plt.xlabel("Latitude")
#plt.xlim(0,139)
#on enregistre le graph
plt.savefig('2.png')
plt.show()


