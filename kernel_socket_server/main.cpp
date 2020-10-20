#include "Utils.h"

extern void NTAPI server_thread(void*);

extern "C" NTSTATUS DriverEntry(DRIVER_OBJECT  driver_object, PUNICODE_STRING registry_path)
{
	KeEnterGuardedRegion();

	ClearPiDDBCacheTable();

	UNICODE_STRING drvName;
	RtlInitUnicodeString(&drvName, L"iqvw64e.sys");

	RetrieveMmUnloadedDriversData();
	ClearMmUnloadedDrivers(&drvName, TRUE);

	UNREFERENCED_PARAMETER(driver_object);
	UNREFERENCED_PARAMETER(registry_path);

	PWORK_QUEUE_ITEM WorkItem = (PWORK_QUEUE_ITEM)ExAllocatePool(NonPagedPool, sizeof(WORK_QUEUE_ITEM));

	ExInitializeWorkItem(WorkItem, server_thread, WorkItem);

	ExQueueWorkItem(WorkItem, DelayedWorkQueue);

	KeLeaveGuardedRegion();

	return STATUS_SUCCESS;
}