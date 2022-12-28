#pragma once

#if defined(__linux__) || defined(__APPLE__)
#include <ctime>
#include <string>
#include <vector>
#include <tuple>
#include <map>
typedef struct timespec TimingVar;
typedef std::vector<uint64_t> &OperationList;
#else
// TODO add option for architecture specific implementation
#endif
typedef uint64_t (*evalType)(OperationList);

class Timing {
public:

    enum TimeUnit {
        SECS = 1,
        MILLI_SECS = 1000,
        MICRO_SECS = 1000000,
        NANO_SECS = 1000000000,
    };

    /**
     * @brief Type of operation to execute on the array of measurements. e.g.
     * TIMING_AVG calculates the average over all measurements in the m_TimeMeasurements vector.
     */
    enum EvalFunctions {
        TIMING_AVG,
        TIMING_MEDIAN,
        TIMING_TOTAL,
        TIMING_MAX,
        TIMING_MIN,
        TIMING_STDDEV
    };

    Timing();
    void start();
    void stop();

    void reset();

    uint64_t getTimeLastMeasurement(TimeUnit timeUnit) const;
    double getTimeLastMeasurementDouble(TimeUnit timeUnit) const;

    uint64_t evaluateMeasureArray(EvalFunctions evalFunctions, Timing::TimeUnit timeUnit);
    double evaluateMeasureArrayDouble(EvalFunctions evalFunctions, TimeUnit timeUnit);

    static std::string getTimeUnitAsStr(TimeUnit timeUnit);

private:
    bool m_Running;
    std::map<EvalFunctions, evalType> m_EvalMap;

    struct CurrentMeasurement {
        TimingVar m_Start;
        TimingVar m_Stop;
    };

    CurrentMeasurement m_CurrentMeasurement;
#if defined(__linux__) || defined(__APPLE__)
    std::vector<CurrentMeasurement> m_TimeMeasurements;
#else
    int m_currentCtr = 0;
	std::array<CurrentMeasurement, MAX_NUMBER_MEASUREMENTS> m_TimeMeasurements;
#endif // __linux__
    static uint64_t transformTimingVarToNs(const TimingVar &tsStart, const TimingVar &tsStop);
    static uint64_t transformNsToUnit(uint64_t time, TimeUnit unit);
    static double transformNsToUnitDouble(uint64_t time, TimeUnit unit);
};
