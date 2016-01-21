/**
 * \class The Wasprogramma user class
 */

#ifndef _WASPROGRAMMA_H
#define _WASPROGRAMMA_H

class Wasprogramma {
private:
	int temp, waterlevel, time, job;
public:
	Wasprogramma(int temp, int waterlevel, int time, int job);

	/*!	\fn		int getLevel()
	 *	\return	Returns the newest requisted waterlevel
	 */
	int getLevel();
	
	/*!	\fn		int getTemp()
	 *	\return	Returns the newest requisted temp
	 */
	int getTemp();
	
	/*!	\fn		int getTime()
	 *	\return	Returns the newest requisted time
	 */
	int getTime();
	
	/*!	\fn		int getJob()
	 *	\return	Returns the newest requisted job
	 */
	int getJob();
};

#endif