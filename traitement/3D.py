from mpl_toolkits.mplot3d import Axes3D
import numpy as np
import matplotlib.pyplot as plt
import os
import pandas as pd
from scipy.stats import ks_2samp
import gmplot
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


fig = plt.figure()
ax = fig.add_subplot(111,projection='3d')
ax.scatter(data_x,data_y,data_z,c='r',marker='o')

ax.set_xlabel('x')
ax.set_ylabel('y')
ax.set_zlabel('z')
plt.show()
