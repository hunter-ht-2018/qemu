#ifndef QEMU_EXEC_LOG_H
#define QEMU_EXEC_LOG_H

#include "qemu/log.h"
#include "qom/cpu.h"
#include "disas/disas.h"

/* cpu_dump_state() logging functions: */
/**
 * log_cpu_state:
 * @cpu: The CPU whose state is to be logged.
 * @flags: Flags what to log.
 *
 * Logs the output of cpu_dump_state().
 */
static inline void log_cpu_state(CPUState *cpu, int flags)
{
    if (qemu_log_enabled()) {
        cpu_dump_state(cpu, qemu_logfile, fprintf, flags);
    }
}

/**
 * log_cpu_state_mask:
 * @mask: Mask when to log.
 * @cpu: The CPU whose state is to be logged.
 * @flags: Flags what to log.
 *
 * Logs the output of cpu_dump_state() if loglevel includes @mask.
 */
static inline void log_cpu_state_mask(int mask, CPUState *cpu, int flags)
{
    if (qemu_loglevel & mask) {
        log_cpu_state(cpu, flags);
    }
}

static inline void log_ins_flow(void * code, unsigned long size)
{

    fwrite(code, 1, size, qemu_logfile);

}

#ifdef NEED_CPU_H
/* disas() and target_disas() to qemu_logfile: */
static inline void log_target_disas(CPUState *cpu, target_ulong start,
                                    target_ulong len)
{
    target_disas(qemu_logfile, cpu, start, len);
}

static inline void log_disas(void *code, unsigned long size)
{
    disas(qemu_logfile, code, size);
}

static inline void log_ins_flow(CPUState *cpu, target_ulong start,
                                    target_ulong len)
{
    uint8_t* buf = (uint8_t*)malloc(len);
    cpu_memory_rw_debug(cpu, start, buf, len, 0);
    fwrite(buf, 1, len, qemu_logfile);
    free(buf);
}


#if defined(CONFIG_USER_ONLY)
/* page_dump() output to the log file: */
static inline void log_page_dump(void)
{
    page_dump(qemu_logfile);
}
#endif
#endif

#endif
