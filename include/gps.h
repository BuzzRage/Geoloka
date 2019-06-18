#ifndef GPS_H
#define GPS_H

//#define DEBUG_GPS
#ifdef DEBUG_GPS
	void displayInfo();
#endif

#define GPS_RX								3
#define GPS_TX								2
#define GPS_BAUD							4800

void initGPS();
void testGPS();

#endif
