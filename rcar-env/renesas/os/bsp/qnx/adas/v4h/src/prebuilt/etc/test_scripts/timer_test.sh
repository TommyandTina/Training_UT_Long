#!/bin/sh

case $1 in
    "cmt")
        env_chan="CMT_FORCE_CHAN"
        num_chan=26
        ;;
    "tmu")
        env_chan="TMU_FORCE_CHAN"
        num_chan=14
        ;;
    *)
        echo "Unknown timer module ($1)"
        exit 1
        ;;
esac

trap sigquit_handler QUIT

function sigquit_handler {
    echo "Script aborted"
    exit 1
}

if [[ -z "${TM_TEST_SEC}" ]]; then
    export TM_TEST_SEC=0
fi

if [[ -z "${TM_TEST_NSEC}" ]]; then
    export TM_TEST_NSEC=100000000
fi

if [[ -z "${TM_TEST_SLEEP}" ]]; then
    export TM_TEST_SLEEP=5
fi

chan_idx=0

echo "Press Ctrl-\ to abort test script\n"

while [[ ${chan_idx} -lt ${num_chan} ]] ; do
    echo "\nStart testing timer channel $chan_idx"
    export ${env_chan}=${chan_idx}
    timer_test -m $1 -t $2 -s ${TM_TEST_SEC} -n ${TM_TEST_NSEC} &
    sleep ${TM_TEST_SLEEP}
    pid=$(pidin ar | grep "timer_test -m" | awk {'print$1'})
    kill -SIGINT ${pid} 2>/dev/null
    wait ${pid}
    (( chan_idx += 1 ))
done

echo "\nTest done!"
