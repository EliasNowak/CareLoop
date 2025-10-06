#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/printk.h>
#include <math.h>

#include "heart_rate.h"
#include "hr_filter.h"


LOG_MODULE_REGISTER(hr_proc, LOG_LEVEL_INF);


#define STACKSIZE 1024
#define THREAD0_PRIORITY 7

static void hr_thread_entry(void *p1, void *p2, void *p3)
{
    // mark as unused
	(void)p1;
	(void)p2;
	(void)p3;

	LOG_INF("HR thread started");

	while (1) {

        //TODO: read sensor signal from hal

        //TODO: filter signal

        //TODO: compute (salience)

        //TODO: beats


		k_sleep(K_MSEC(100)); /* placeholder delay */
	}
}

K_THREAD_DEFINE(hr_thread_id, STACKSIZE, hr_thread_entry, NULL, NULL, NULL,
				THREAD0_PRIORITY, 0, 0);

