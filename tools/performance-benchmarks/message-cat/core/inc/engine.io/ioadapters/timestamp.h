#pragma once

typedef struct TS {
	int tsId;
	LARGE_INTEGER frequency;        // ticks per second
	LARGE_INTEGER t1, t2;           // ticks
	double elapsedTime;

	TS(int id) : tsId(id) {
		// get ticks per second
		QueryPerformanceFrequency(&frequency);

		// start timer
		QueryPerformanceCounter(&t1);
	}

} TS;

static std::vector<TS*> timeStamps;

static int time_start()
{
	static int tsSessions = 0;
	int timeStampId = tsSessions++;
	TS *ts = new TS(timeStampId);
	timeStamps.push_back(ts);

	return timeStampId;
}

static double time_end(int timeStampId)
{
	TS *ts = timeStamps[timeStampId];

	// stop timer
	QueryPerformanceCounter(&ts->t2);

	// compute and print the elapsed time in millisec
	ts->elapsedTime = (ts->t2.QuadPart - ts->t1.QuadPart) * 1000.0 / ts->frequency.QuadPart;

	return ts->elapsedTime;
}
