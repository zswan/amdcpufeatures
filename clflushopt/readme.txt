Test clflush:

 

# insmod clflush_test_driver.ko

[   15.663075] clflush_test_driver: module verification failed: signature and/or required key missing - tainting kernel

[   15.663569] CPU supports CLFLUSH feature now!

[   15.663675] CPU supports CLFLUSHopt feature now!

[   15.663787] We will disbale clflushopt feature now!!!

[   15.663909] Called into clflush_cache_range, buff virt addr:0xffff88003714a000, x86_clflush_size=64

 

 Test clflushopt:

 

# insmod clflush_test_driver.ko

[   62.533008] clflush_test_driver: module verification failed: signature and/or required key missing - tainting kernel

[   62.533495] CPU supports CLFLUSH feature now!

[   62.533601] CPU supports CLFLUSHopt feature now!

[   62.533713] We will enable clflushopt feature now!!!

[   62.533835] Called into clflush_cache_range, buff virt addr:0xffff88003714a000, x86_clflush_size=64

 

 so clflush and clflushopt work ok at lastest kernel version.
