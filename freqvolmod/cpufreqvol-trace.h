#undef TRACE_SYSTEM
#define TRACE_SYSTEM cpufreqvol

#if !defined(_CPUSTATS_TRACE_H) || defined(TRACE_HEADER_MULTI_READ)
#define _CPUSTATS_TRACE_H

#include <linux/tracepoint.h>

TRACE_EVENT(get_stats,

	TP_PROTO(int voltage, int frequency),

	TP_ARGS(voltage, frequency),

	TP_STRUCT__entry(
		__field(	int, voltage		)
		__field(	int, frequency 	)
	),

	TP_fast_assign(
		__entry->voltage = voltage;
		__entry->frequency = frequency;
	),

	TP_printk("voltage=%d frequency=%d",
		__entry->voltage, __entry->frequency)
);

#endif /* _GET_STATS_TRACE_H */

/* This part must be outside protection */
#undef TRACE_INCLUDE_PATH
#define TRACE_INCLUDE_PATH .
#define TRACE_INCLUDE_FILE cpufreqvol-trace
#include <trace/define_trace.h>