#include "ctlib/Timing.hpp"

#if __linux__
#include <iostream>
#include <numeric>
#include <algorithm>
#endif

Timing::Timing() : m_Running(false),
                   m_EvalMap({{TIMING_AVG,
                                              [](OperationList list) -> uint64_t {
                                                  uint64_t sum_of_elems = 0;
                                                  return std::accumulate(list.begin(), list.end(), sum_of_elems, std::plus<uint64_t>())
                                                         / list.size();
                                              }},

                              {TIMING_TOTAL,
                                              [](OperationList list) -> uint64_t {
                                                  uint64_t sum_of_elems = 0;
                                                  return std::accumulate(list.begin(), list.end(), sum_of_elems, std::plus<uint64_t>());
                                              }},

                              {TIMING_MAX,
                                              [](OperationList list) -> uint64_t {
                                                  std::sort(list.begin(), list.end());
                                                  return list.back();
                                              }},

                              {TIMING_MIN,
                                              [](OperationList list) -> uint64_t {
                                                  std::sort(list.begin(), list.end());
                                                  return list.front();
                                              }},

                              {TIMING_MEDIAN, [](OperationList list) -> uint64_t {
                                  std::sort(list.begin(), list.end());
                                  if (list.size() % 2 == 0)
                                      return (list[list.size() / 2] + list[list.size() / 2 + 1]) / 2;
                                  return list[list.size() / 2];
                              }}}),

                   m_CurrentMeasurement({{0, 0}, {0, 0}}) {
}

/**
 * @brief Starts the timing measurements. If there is already a timing measurement running stop the previous one,
 * store the result on the time measurement vector and restart the measurement.
 */
void Timing::start() {
#ifdef __linux__
    if (m_Running)
        stop();
    m_Running = true;
    if (clock_gettime(CLOCK_MONOTONIC_RAW, &m_CurrentMeasurement.m_Start) != 0)
        std::cout << "Error while calling \"clock_gettime()\"" << std::endl;
#else
    // TODO add option for architecture specific implementation
#endif
}

/**
 * @brief Stop the measurement and append the result to the timeMeasurements vector.
 */
void Timing::stop() {
    if(!m_Running)
        return;
    m_Running = false;
#ifdef __linux__
    if (clock_gettime(CLOCK_MONOTONIC_RAW, &m_CurrentMeasurement.m_Stop) != 0)
        std::cout << "Error while calling \"clock_gettime()\"" << std::endl;
    m_TimeMeasurements.emplace_back(m_CurrentMeasurement);
#else
    // TODO add option for architecture specific implementation
#endif
}

/**
 * @brief Stop all running measurements and clear the list of measurements.
 */
void Timing::reset() {
    if (m_Running)
        stop();
#ifdef __linux__
    m_TimeMeasurements.clear();
#else
    m_currentCtr = 0;
#endif
}

/**
 * @brief Get the time in timeUnit's as integer from the last measurement in the measurement vector.
 * @param timeUnit unit such as nano seconds, milli seconds or seconds.
 * @return time in timeUnit as integer
 */
uint64_t Timing::getTimeLastMeasurement(Timing::TimeUnit timeUnit) const {
    return transformNsToUnit(transformTimingVarToNs(m_CurrentMeasurement.m_Start, m_CurrentMeasurement.m_Stop), timeUnit);
}

/**
 * @brief Get the time in timeUnit's as double from the last measurement in the measurement vector.
 * @param timeUnit unit such as nano seconds, milli seconds or seconds.
 * @return time in timeUnit as integer
 */
double Timing::getTimeLastMeasurementDouble(Timing::TimeUnit timeUnit) const {
    return transformNsToUnitDouble(getTimeLastMeasurement(NANO_SECS), timeUnit);
}

/**
 * @brief Execute a passed evaluation algorithm, e.g. average or median on the list of all timing measurements
 * as 64-bit integer.
 * @param evalFunctions function to execute on the array.
 * @param timeUnit unit such as nano seconds, milli seconds or seconds.
 * @return evaluated result.
 */
uint64_t Timing::evaluateMeasureArray(EvalFunctions evalFunctions, Timing::TimeUnit timeUnit) {
    std::vector<uint64_t> timesInNs;

    for (auto elem : m_TimeMeasurements) {
        auto t = Timing::transformTimingVarToNs(elem.m_Start, elem.m_Stop);
        timesInNs.push_back(t);
    }
    uint64_t value = m_EvalMap.at(evalFunctions)(timesInNs);

    return transformNsToUnit(value, timeUnit);
}

/**
 * @brief Execute a passed evaluation algorithm, e.g. average or median on the list of all timing measurements
 * as double.
 * @param evalFunctions function to execute on the array.
 * @param timeUnit unit such as nano seconds, milli seconds or seconds.
 * @return evaluated result.
 */
double Timing::evaluateMeasureArrayDouble(EvalFunctions evalFunctions, Timing::TimeUnit timeUnit) {
    return transformNsToUnitDouble(evaluateMeasureArray(evalFunctions, NANO_SECS), timeUnit);
}

/**
 * @brief Return the passed time unit as string
 * @param timeUnit unit such as nano seconds, milli seconds or seconds.
 * @return time unit as string, e.g. s for seconds or ns for nano seconds
 */
/*static*/ std::string Timing::getTimeUnitAsStr(TimeUnit timeUnit) {
    switch (timeUnit) {
        case SECS: return "s";
        case MILLI_SECS: return "ms";
        case MICRO_SECS: return "us";
        case NANO_SECS: return "ns";
    }
    return "Invalid Time Unit";
}

/**
 * @brief Transform the timestamp of TimingVar on linux implemented as timespec to nano seconds.
 * @param tsStart start timestamp.
 * @param tsStop stop timestamp.
 * @return difference between start and stop timestamp in nanoseconds encoded as 64-bit integer.
 */
/*static*/ uint64_t Timing::transformTimingVarToNs(const TimingVar &tsStart, const TimingVar &tsStop) {
#ifdef __linux__
    if (tsStart.tv_nsec <= tsStop.tv_nsec)
        return ((tsStop.tv_sec - tsStart.tv_sec) * NANO_SECS) + (tsStop.tv_nsec - tsStart.tv_nsec);
    else {
        uint64_t secs = (tsStop.tv_sec - tsStart.tv_sec) - 1;
        uint64_t nanoSecs = (NANO_SECS - tsStart.tv_nsec) + tsStop.tv_nsec;
        return secs * NANO_SECS + nanoSecs;
    }
#else
    return m_Stop-m_Start;
#endif
}

/**
 * @brief Transforms a 64-bit integer as nano second to a specific unit e.g. seconds.
 * @param time time in nanoseconds
 * @param unit unit, e.g. seconds or nano seconds.
 * @return time encoded in specific unit.
 */
/*static*/ uint64_t Timing::transformNsToUnit(uint64_t time, Timing::TimeUnit unit) {
    return time / static_cast<uint64_t>(NANO_SECS / unit);
}

/**
 * @brief Transforms a 64-bit integer as nano second to a specific unit e.g. seconds as double value.
 * @param time time in nanoseconds
 * @param unit unit, e.g. seconds or nano seconds.
 * @return time encoded in specific unit.
 */
double Timing::transformNsToUnitDouble(uint64_t time, Timing::TimeUnit unit) {
    return static_cast<double>(time) / (static_cast<double>(NANO_SECS) / unit);
}