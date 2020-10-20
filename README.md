# KSOCKET Process Memory Manager


##### The original KSOCKET provides a very basic example on how to make network connections in the Windows Driver by using WSK.
Link to the original KSOCKET : https://github.com/wbenny/KSOCKET


In this project, I am modifying a bit the original KSOCKET, so I can manage protected process memory of applications.
This can apply in a variety of cases. One of these is to read  and write to the process memory of games protected by anti-cheats that strip the capability of getting a valid access handle to the game.


### Extras

The compiled driver is unsigned. Thus, we can not just start it as a service, which is what we'd probably do with a trusted signed .sys file.

The only ways of loading it into the system would be to either disable Windows DSE (Driver Signature Enforcement)
or manual map it using another vulnerable driver.

In our case, we need DSE on, as most of the anti-cheats won't launch when DSE is set to off.

The driver is manually mapped using Intel's "iqvw64e.sys". 
Then "iqvw64e.sys" is unloaded and we're ready to play.
The mapper I used is https://github.com/alxbrn/kdmapper-1803-1903
which does all I mentioned above automatically.

Though, Intel's driver is blacklisted from many anti-cheats just for that reason, that cheaters can manually map their memory management driver,
thus the first functions we call in our DriverEntry are:
```C
ClearPiDDBCacheTable();

UNICODE_STRING drvName;
RtlInitUnicodeString(&drvName, L"iqvw64e.sys");

RetrieveMmUnloadedDriversData();
ClearMmUnloadedDrivers(&drvName, TRUE);
```
which will clean the PiDDB Cache Table and any recently unloaded drivers of the system (which in our case is the blacklisted iqvw64e.sys).


## PoC
![PoC](https://i.imgur.com/CTADSHW.png)