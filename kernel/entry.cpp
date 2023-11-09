#include "library/globals.h"


UNICODE_STRING dev_name, sym_link;

DRIVER_UNLOAD DriverUnload;

void DriverUnload( _DRIVER_OBJECT* DriverObject
)
{
	IoDeleteSymbolicLink(&sym_link);
	IoDeleteDevice(DriverObject->DeviceObject);
}


		
extern "C" NTSTATUS DriverEntry(IN PDRIVER_OBJECT driver_obj, IN PUNICODE_STRING  registery_path)
{
	///////////////////////////////
	UNREFERENCED_PARAMETER(registery_path);
	///////////////////////////////

	///////////////////////////////
	PDEVICE_OBJECT dev_obj;
	NTSTATUS status = STATUS_SUCCESS;
	///////////////////////////////

	///////////////////////////////
	status = init_function();
	if (!NT_SUCCESS(status))
		return status;
	///////////////////////////////

	///////////////////////////////

	dev_name = ansi_to_unicode("\\Device\\Driver");
	sym_link = ansi_to_unicode("\\DosDevices\\Driver");
	///////////////////////////////

	///////////////////////////////
	status = IoCreateDevice(driver_obj, 0, &dev_name, file_device_mirrore, 0x00000100, 0, &dev_obj);
	if (!NT_SUCCESS(status))
		return status;
	///////////////////////////////

	///////////////////////////////
	status = IoCreateSymbolicLink(&sym_link, &dev_name);
	if (!NT_SUCCESS(status))
		return status;
	///////////////////////////////

	///////////////////////////////
	SetFlag(dev_obj->Flags, DO_BUFFERED_IO);

	for (int t = 0; t <= IRP_MJ_MAXIMUM_FUNCTION; t++)
		driver_obj->MajorFunction[t] = unsupported_io;

	driver_obj->MajorFunction[IRP_MJ_CREATE] = create_io;
	driver_obj->MajorFunction[IRP_MJ_CLOSE] = close_io;
	driver_obj->MajorFunction[IRP_MJ_DEVICE_CONTROL] = ctl_io;
	driver_obj->DriverUnload = DriverUnload;

	ClearFlag(dev_obj->Flags, DO_DEVICE_INITIALIZING);
	///////////////////////////////

	return status;
}

