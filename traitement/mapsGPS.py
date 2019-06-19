import numpy as np
import matplotlib.pyplot as plt
import os
import pandas as pd
from scipy.stats import ks_2samp
import gmplot

#chargement csv
data_csv = pd.read_csv(os.path.join(os.getcwd(), 'vdb_fixe.csv'), names=['Date', 'Heure', 'Latitude', 'Longitude','Altitude','NBsat','HDOP'])
#enregistre des angles
data_date = data_csv['Date'].values[1:]
data_heure = data_csv['Heure'].values[1:]
data_lat = [float(x) for x in data_csv['Latitude'].values[1:]]
data_long = [float(x) for x in data_csv['Longitude'].values[1:]]

gmap3 = gmplot.GoogleMapPlotter(data_lat[1],data_long[1] , 13) 
gmap3.apikey="AIzaSyASykRT1nBDi3zDKbHYpmSy1nTMagWObLI"
gmap3.scatter( data_lat, data_long, '# FF0000',size = 40, marker = False ) 
  
# Plot method Draw a line in 
# between given coordinates 
gmap3.plot(data_lat, data_long,'cornflowerblue', edge_width = 2.5) 
  
gmap3.draw( "map1.html" ) 

