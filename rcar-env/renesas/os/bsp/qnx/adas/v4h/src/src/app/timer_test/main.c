/*
 * Demonstrate how to set up a timer that, on expiry, 
 * sends us a pulse.  This example sets the first 
 * expiry to 1.5 seconds and the repetition interval 
 * to 1.5 seconds.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <getopt.h>
#include <sys/mman.h>
#include <sys/netmgr.h>
#include <sys/neutrino.h>
#include <sys/syspage.h>
#include <hw/rcar_cpg.h>
#include <hw/rcar_timer.h>

#define TMU_MOD_COUNT      5
#define CMT_MOD_COUNT      4

#define TEST_OPT_MOD_TMU   "tmu"
#define TEST_OPT_MOD_CMT   "cmt"

#define TEST_OPT_GETTIME   "gettime"
#define TEST_OPT_SETTIME   "settime"

#define TEST_MODE_SETTIME  (1)
#define TEST_MODE_GETTIME  (2)

#define TEST_ERR_THRESH    (1.0)
#define TEST_PULSE_CODE     _PULSE_CODE_MINAVAIL

typedef union {
    struct _pulse pulse;
} tm_msg_t;

typedef struct tm_data {
    struct itimerspec itime;
} tm_data_t;

static rcar_timer_functions_t timer_hwfuncs = { 0 };
static int quit = 0;
static void *tm_hdl;

void timespec_diff(struct itimerspec *start, struct itimerspec *stop, struct itimerspec *result)
{
    if((stop->it_value.tv_nsec - start->it_value.tv_nsec) < 0) {
        result->it_value.tv_sec = stop->it_value.tv_sec - start->it_value.tv_sec - 1;
        result->it_value.tv_nsec = stop->it_value.tv_nsec - start->it_value.tv_nsec + 1000000000;
    } else {
        result->it_value.tv_sec = stop->it_value.tv_sec - start->it_value.tv_sec;
        result->it_value.tv_nsec = stop->it_value.tv_nsec - start->it_value.tv_nsec;
    }
}

void signal_handler(int signum)
{
    switch(signum) {
        case SIGINT:
        case SIGQUIT:
        case SIGTERM:
        case SIGKILL:
        case SIGSEGV:
            quit = 1;
            break;
        default:
            break;
    }
}

int timer_test_settime(tm_data_t *data)
{
    struct sigevent   event;
    int               chid, rcvid;
    double            elapse, differ;
    uint64_t          actual, expect;
    uint64_t          timeout;
    uint64_t          cc_start;
    uint64_t          cc_stop;
    uint64_t          n_overflow = 0;;
    tm_msg_t          msg;

    chid = ChannelCreate(0);

    event.sigev_notify = SIGEV_PULSE;
    event.sigev_coid = ConnectAttach(ND_LOCAL_NODE, 0, chid, _NTO_SIDE_CHANNEL, 0);
    event.sigev_code = TEST_PULSE_CODE;
    event.sigev_priority = 10;

    timeout = 2 * (data->itime.it_value.tv_sec * 1000000000 + data->itime.it_value.tv_nsec);

    if(!(tm_hdl = timer_hwfuncs.timer_create(&event))) {
        fprintf(stderr, "Unable to create timer\n");
        return -1;
    }

    if(timer_hwfuncs.timer_settime(tm_hdl, &data->itime) < 0) {
        fprintf(stderr, "Unable to set timer value\n");
        timer_hwfuncs.timer_delete(tm_hdl);
        return -1;
    }

    timer_hwfuncs.timer_start(tm_hdl);

    cc_start = ClockCycles();

    fprintf(stdout, "Wait a moment, then press Ctrl-C to stop the test\n");

    while(1) {
        TimerTimeout(CLOCK_MONOTONIC, _NTO_TIMEOUT_RECEIVE, NULL, &timeout, NULL);
        if((rcvid = MsgReceive(chid, &msg, sizeof(msg), NULL)) < 0) {
            if(errno == ETIMEDOUT) {
                fprintf(stderr, "Timeout waiting timer interrupt\n");
                break;
            }
        } else if(!rcvid) {
            if(msg.pulse.code == TEST_PULSE_CODE) {
                n_overflow++;
                if(quit) { break; }
            }
        }
    }

    cc_stop = ClockCycles();

    elapse = (cc_stop - cc_start) / (double)SYSPAGE_ENTRY(qtime)->cycles_per_sec;
    actual = (elapse / n_overflow) * 1000000000;
    expect = data->itime.it_value.tv_sec * 1000000000 + data->itime.it_value.tv_nsec;
    differ = 100.f * labs(actual - expect) / expect;

    fprintf(stdout, "Test result in %.2lf sec\n", elapse);
    fprintf(stdout, "  Num Overflow = %ld\n", n_overflow);
    fprintf(stdout, "      Expected = %ld sec, %ld nsec\n", data->itime.it_value.tv_sec, data->itime.it_value.tv_nsec);
    fprintf(stdout, "      Measured = %ld sec, %ld nsec\n", actual / 1000000000, actual % 1000000000);
    fprintf(stdout, "    Difference = %.3lf%% (%ld nsec)\n", differ, labs(actual - expect));

    timer_hwfuncs.timer_stop(tm_hdl);
    timer_hwfuncs.timer_delete(tm_hdl);

    return (differ < TEST_ERR_THRESH) ? 0 : -1;
}

int timer_test_gettime(tm_data_t *data)
{
    struct itimerspec tm_start;
    struct itimerspec tm_stop;
    struct itimerspec tm_diff;
    uint64_t          start, stop;
    uint64_t          expect, actual;
    double            differ;

    if(!(tm_hdl = timer_hwfuncs.timer_create(NULL))) {
        fprintf(stderr, "Unable to create timer\n");
        return -1;
    }

    timer_hwfuncs.timer_start(tm_hdl);
    timer_hwfuncs.timer_gettime(tm_hdl, &tm_start);

    start = ClockCycles();

    fprintf(stdout, "Wait a moment, then press Ctrl-C to stop the test\n");

    while(!quit) {
        usleep(10000);
    }

    timer_hwfuncs.timer_gettime(tm_hdl, &tm_stop);

    stop = ClockCycles();

    timer_hwfuncs.timer_stop(tm_hdl);
    timer_hwfuncs.timer_delete(tm_hdl);

    timespec_diff(&tm_start, &tm_stop, &tm_diff);

    expect = (uint64_t)(1000000000 * (double)(stop - start) / (SYSPAGE_ENTRY(qtime)->cycles_per_sec));
    actual = tm_diff.it_value.tv_sec * 1000000000 + tm_diff.it_value.tv_nsec;
    differ = 100.f * labs(actual - expect) / expect;

    fprintf(stdout, "Measured result\n");
    fprintf(stdout, "    Expected = %ld sec, %ld nsec\n", expect / 1000000000, expect % 1000000000);
    fprintf(stdout, "    Measured = %ld sec, %ld nsec\n", tm_diff.it_value.tv_sec, tm_diff.it_value.tv_nsec);
    fprintf(stdout, "  Difference = %.3lf%% (%ld nsec)\n", differ, labs(actual - expect));

    return (differ < TEST_ERR_THRESH) ? 0 : -1;
}

int main(int argc, char *argv[])
{
    int opt, index, rc = 0;
    int mode = TEST_MODE_SETTIME;
    int module = TIMER_TMU_MOD;

    tm_data_t data = {
        .itime = {
            .it_value = { 0, 100000000 }, 
            .it_interval = { 0, 100000000 }
        }
    };

    static struct option options[] = {
        { "module",       required_argument, 0,  'm' },
        { "tmode",        required_argument, 0,  't' },
        { "period_sec",   required_argument, 0,  's' },
        { "period_nsec",  required_argument, 0,  'n' },
        { NULL,           0,                 0,   0  }
    };

    while((opt = getopt_long(argc, argv,"m:t:s:n:", options, &index )) != -1) {
        switch (opt) {
            case 't':
                if(!strcmp(optarg, TEST_OPT_GETTIME)) {
                    mode = TEST_MODE_GETTIME;
                } else if(!strcmp(optarg, TEST_OPT_SETTIME)) {
                    mode = TEST_MODE_SETTIME;
                } else {
                    fprintf(stderr, "Unsupported timer mode (%s)\n", optarg);
                    rc = -1; goto exit;
                }
                break;
            case 'm':
                if(!strcmp(optarg, TEST_OPT_MOD_TMU)) {
                    module = TIMER_TMU_MOD;
                } else if(!strcmp(optarg, TEST_OPT_MOD_CMT)) {
                    module = TIMER_CMT_MOD;
                } else {
                    fprintf(stderr, "Unsupported timer module (%s)\n", optarg);
                    rc = -1; goto exit;
                }
                break;
            case 's':
                data.itime.it_value.tv_sec = strtol(optarg, NULL, 10);
                data.itime.it_interval.tv_sec = data.itime.it_value.tv_sec;
                break;
            case 'n':
                data.itime.it_value.tv_nsec = strtol(optarg, NULL, 10);
                data.itime.it_interval.tv_nsec = data.itime.it_value.tv_nsec;
                break;
            default:
                fprintf(stdout, "Unknown option\n");
                rc = -1; goto exit;
        }
    }

    signal(SIGINT,  signal_handler);
    signal(SIGQUIT, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGKILL, signal_handler);
    signal(SIGSEGV, signal_handler);

    if((rc = rcar_timer_get_funcs(module, &timer_hwfuncs, sizeof(rcar_timer_functions_t))) != EOK) {
        fprintf(stderr, "Unable to get timer hw funcs (%s)\n", strerror(rc));
        rc = -1; goto exit;
    }

    if(mode == TEST_MODE_GETTIME) {
        rc = timer_test_gettime(&data);
    } else {
        rc = timer_test_settime(&data);
    }

exit:
    if(rc < 0) {
        fprintf(stdout, "\033[1;31mFailed\033[0;0m\n");
    } else {
        fprintf(stdout, "\033[1;32mPassed\033[0;0m\n");
    }

    return rc ? EXIT_FAILURE : EXIT_SUCCESS;
}
