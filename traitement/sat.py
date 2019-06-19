import numpy as np
import matplotlib.pyplot as plt
import os
import pandas as pd
from scipy.stats import ks_2samp
import gmplot
import numpy

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
data_NBsat = [float(x) for x in data_csv['NBsat'].values[1:]]
data_HDOP = [float(x) for x in data_csv['HDOP'].values[1:]]

#on affiche NBsat

plt.subplot(211)
plt.plot(data_second,data_NBsat,'b-',label='e')
plt.grid()
plt.ylabel("Nombre de satellites")
plt.xlabel("Temps(s)")

#on affiche HDOP

plt.subplot(212)
plt.plot(data_second,data_HDOP,'b-',label='e')
plt.grid()
plt.ylabel("HDOP")
plt.xlabel("Temps(s)")
plt.show()
