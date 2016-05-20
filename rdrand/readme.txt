Test result:


zswan@zswan-developer:~/testapp/rdrand$ sudo ./rdrand_test.sh
[sudo] password for zswan:
----------> First step,make clean and make driver modules!
make -C /lib/modules/3.19.0-smep-autotest/build SUBDIRS=/home/zswan/testapp/rdrand clean
make[1]: Entering directory `/home/zswan/linux'
  CLEAN   /home/zswan/testapp/rdrand/.tmp_versions
  CLEAN   /home/zswan/testapp/rdrand/Module.symvers
make[1]: Leaving directory `/home/zswan/linux'
rm -rf Module.markers modules.order Module.symvers
make -C /lib/modules/3.19.0-smep-autotest/build SUBDIRS=/home/zswan/testapp/rdrand modules
make[1]: Entering directory `/home/zswan/linux'
  CC [M]  /home/zswan/testapp/rdrand/rdrand_test_driver.o
  Building modules, stage 2.
  MODPOST 1 modules
  CC      /home/zswan/testapp/rdrand/rdrand_test_driver.mod.o
  LD [M]  /home/zswan/testapp/rdrand/rdrand_test_driver.ko
make[1]: Leaving directory `/home/zswan/linux'
rdrand_test_driver     12626  0
----------> Second step, check driver modules, existing!
rmmod rdrand_test_driver modules Ok!!!
insmod /home/zswan/testapp/rdrand/rdrand_test_driver.ko modules Ok!!!
[179925.371080] Ok to install rdrand test_driver
[179925.371933] Start to test 64bit rdrand.
[179925.377040] ok!!!, CPU 64bit rdrand feature work ok, test done!
[179925.377233] Start to test 32bit rdrand.
[179925.387113] ok!!!, CPU 32bit rdrand feature work ok, test done!
[179925.387329] Start to test 64bit rdrand.
[179925.392456] ok!!!, CPU 64bit rdrand feature work ok, test done!
[179925.392686] Start to test 32bit rdrand.
[179925.402536] ok!!!, CPU 32bit rdrand feature work ok, test done!
